// Copyright GGBAO 


#include "GAS/ExecCalcs/ExecCalcDamage.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/CharacterClassInfo.h"
#include "Interfaces/CombatInterface.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(UExecCalcDamageLog, All, All);

/// 定义用于捕获的属性结构,辅助自定义计算使用,不用暴露给蓝图
struct EffectAttributeCapture
{
	// 声明属性捕获结构,这里的Armor可以是任意值,只是变量的名字罢了
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)

	EffectAttributeCapture()
	{
		// 定义属性捕获结构,必须先声明才能定义
		// 比如你想创建捕获护甲的结构数据用于捕获,那就先DECLARE..(Armor)再DEFINE...(...,Armor,xxx)
		// S:你的属性集
		// P:属性集中的属性,要和DECLARE_ATTRIBUTE_CAPTUREDEF宏括号里的参数一致
		// T:Source或者Target (对源还是对目标捕获,这里是对目标的护甲属性进行捕获)
		// B:是否进行快照
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false); // 捕获目标的护甲属性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BlockChance, Target, false); // 捕获目标的格挡属性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration, Target, false); // 捕获目标的护甲穿透属性
	}
};

static EffectAttributeCapture DamageAttributeCapture()
{
	static const EffectAttributeCapture DamageCaptures;
	return DamageCaptures;
}

UExecCalcDamage::UExecCalcDamage()
{
	RelevantAttributesToCapture.Add(DamageAttributeCapture().ArmorDef);
	RelevantAttributesToCapture.Add(DamageAttributeCapture().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageAttributeCapture().ArmorPenetrationDef);
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 从GE里获取源、目标的CombineTags
	const auto SourceTags = ExecutionParams.GetOwningSpec().CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = ExecutionParams.GetOwningSpec().CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// 转换成战斗接口
	const auto SourceCombatInt = Cast<ICombatInterface>(
		ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor());
	const auto TargetCombatInt = Cast<ICombatInterface>(
		ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor());

	if (!SourceCombatInt || !TargetCombatInt)
	{
		UE_LOG(UExecCalcDamageLog, Error, TEXT("[%s]获取化身角色失败!"), *GetName());
		return;
	}

	// 获取GameInstance子系统
	const auto GiSubSystem = Cast<URPGAuraGameInstanceSubsystem>(
		USubsystemBlueprintLibrary::GetGameInstanceSubsystem(
			ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor(),
			URPGAuraGameInstanceSubsystem::StaticClass()));
	if (!GiSubSystem)
	{
		UE_LOG(UExecCalcDamageLog, Error, TEXT("[%s]获取GameInstance子系统失败!"), *GetName());
		return;
	}

	// 获取曲线表
	const auto RealCurveArmorPenetration = GiSubSystem->CharacterClassInfo.Get()->DamageCalculationFactors->FindCurve(
		FName("ArmorPenetrationFactor"), FString());
	const auto RealCurveEffectiveArmor = GiSubSystem->CharacterClassInfo.Get()->DamageCalculationFactors->FindCurve(
		FName("EffectiveArmorFactor"), FString());

	if (!RealCurveArmorPenetration || !RealCurveEffectiveArmor)
	{
		UE_LOG(UExecCalcDamageLog, Error, TEXT("[%s]获取曲线表失败!"), *GetName());
		return;
	}


	// 获取曲线表中的数值 , 获取源的护甲穿透系数
	const float ArmorPenetrationFactor = RealCurveArmorPenetration->Eval(SourceCombatInt->GetCharacterLevel());
	// 获取目标的护甲值系数
	const float EffectiveArmorFactor = RealCurveEffectiveArmor->Eval(TargetCombatInt->GetCharacterLevel());


	// 护甲值
	float ArmorMagnitude = 0.f;

	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAttributeCapture().ArmorDef, EvaluateParameters,
	                                                           ArmorMagnitude);
	ArmorMagnitude = FMath::Max<float>(ArmorMagnitude, 0.f);


	// 获取之前以Abilities_Damage_FireBolt游戏标签为键分配给SetByCaller的伤害值 (注意,我们在发射火球的能力的cpp里用能力系统分配设置了这个键值对,所以能获取到)
	float Damage = ExecutionParams.GetOwningSpec().GetSetByCallerMagnitude(
		FRPGAuraGameplayTags::Get().Abilities_Damage_FireBolt);

	// 格挡几率
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAttributeCapture().BlockChanceDef,
	                                                           EvaluateParameters, TargetBlockChance);


	// 护甲穿透率
	float TargetArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageAttributeCapture().ArmorPenetrationDef,
	                                                           EvaluateParameters, TargetArmorPenetration);

	// 有效护甲计算
	float EffectiveArmor = ArmorMagnitude * ((100 - TargetArmorPenetration * ArmorPenetrationFactor) / 100.f);
	EffectiveArmor = FMath::Max<float>(EffectiveArmor, 0.f);

	// 一点有效护甲抵扣一点伤害
	Damage *= (100 - EffectiveArmor * EffectiveArmorFactor) / 100.f;
	// 根据格挡几率判断是否格挡
	Damage = (FMath::RandRange(1, 100) <= TargetBlockChance) ? Damage * (0.5f) : Damage;

	UE_LOG(UExecCalcDamageLog, Warning,
	       TEXT(
		       "[ArmorPenetration-F]:%.2f , [EffectiveArmor-F]:%.2f  ,[Armor]:%.2f ,[ArmorPenetration]:%.2f ,  [EffectiveArmor]:%.2f , [TargetBlockChance]:%.2f , [Damage]: %.2f"
	       ), ArmorPenetrationFactor, EffectiveArmorFactor, ArmorMagnitude, TargetArmorPenetration,
	       EffectiveArmor, TargetBlockChance, Damage);

	// 修改属性集中名为InComingDamage的属性值
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetInComingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
