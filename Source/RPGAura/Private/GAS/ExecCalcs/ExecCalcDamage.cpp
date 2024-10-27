// Copyright GGBAO 


#include "GAS/ExecCalcs/ExecCalcDamage.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "CoreTypes/RPGAuraGasCoreTypes.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/CharacterClassInfo.h"
#include "GAS/Globals/GameAbilitySystemGlobals.h"
#include "Interfaces/CombatInterface.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(UExecCalcDamageLog, All, All);

/// 定义用于捕获的属性结构,辅助自定义计算使用,不用暴露给蓝图
struct FEffectAttributeCapture
{
	// 声明属性捕获结构,这里的Armor可以是任意值,只是变量的名字罢了
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Intelligence)

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightingResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagToCaptureDefsMap;

	FEffectAttributeCapture()
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
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitChance, Source, false); // 捕获源 的暴击率
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitDamage, Source, false); // 捕获源 的暴击伤害
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitResistance, Target, false); // 捕获 目标的暴击抵抗率
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Intelligence, Source, false); // 捕获 源的智力值

		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, FireResistance, Target, false); // 捕获目标的火焰抗性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalResistance, Target, false); // 捕获目标的物理抗性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, LightingResistance, Target, false); // 捕获目标的雷电抗性
		DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArcaneResistance, Target, false); // 捕获目标的奥术 抗性
	}

	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& GetMap() { return TagToCaptureDefsMap; }
};

static FEffectAttributeCapture GetStaticStruct()
{
	static FEffectAttributeCapture EffectAttributeCapture;
	return EffectAttributeCapture;
}

UExecCalcDamage::UExecCalcDamage()
{
	RelevantAttributesToCapture.Add(GetStaticStruct().ArmorDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().BlockChanceDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().IntelligenceDef);

	RelevantAttributesToCapture.Add(GetStaticStruct().FireResistanceDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().LightingResistanceDef);
	RelevantAttributesToCapture.Add(GetStaticStruct().ArcaneResistanceDef);
}

void UExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                             FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	if (!ExecutionParams.GetSourceAbilitySystemComponent() || !ExecutionParams.GetTargetAbilitySystemComponent())
	{
		return;
	}
	auto GeSpec = ExecutionParams.GetOwningSpec();
	const auto CaptureAttributes = GetStaticStruct();

	// 从GE里获取源、目标的CombineTags
	const auto SourceTags = GeSpec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = GeSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	const auto SourceAvatar = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
	const auto TargetAvatar = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();


	auto GeContextHandle = GeSpec.GetContext();
	const auto MyGeContext = static_cast<FRPGAuraGameplayEffectContext*>(GeContextHandle.Get());

	if (!MyGeContext)
	{
		UE_LOG(UExecCalcDamageLog, Error, TEXT("[%s]获取GE上下文失败!"), *GetName());
		return;
	}
	// 获取源的属性集
	const auto SourceAs = Cast<UBaseAttributeSet>(
		ExecutionParams.GetSourceAbilitySystemComponent()->GetAttributeSet(UBaseAttributeSet::StaticClass()));
	if (!SourceAs)
	{
		UE_LOG(UExecCalcDamageLog, Error, TEXT("[%s]获取源的属性集失败!"), *GetName());
		return;
	}

	// 转换成战斗接口
	const auto SourceCombatInt = Cast<ICombatInterface>(
		SourceAvatar);
	const auto TargetCombatInt = Cast<ICombatInterface>(
		TargetAvatar);

	if (!SourceCombatInt || !TargetCombatInt)
	{
		UE_LOG(UExecCalcDamageLog, Error, TEXT("[%s]转化身角色为接口失败!"), *GetName());
		return;
	}

	// 获取GameInstance子系统
	const auto GiSubSystem = Cast<URPGAuraGameInstanceSubsystem>(
		USubsystemBlueprintLibrary::GetGameInstanceSubsystem(
			SourceAvatar,
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
	float TargetArmor = 0.f;
	// 捕获目标护甲值
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.ArmorDef,
	                                                           EvaluateParameters,
	                                                           TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	// 捕获智力值
	float SourceIntelligence = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.IntelligenceDef,
	                                                           EvaluateParameters,
	                                                           SourceIntelligence);


	// 获得基础伤害
	// 是游戏标签为键分配给SetByCaller的伤害值 (注意,我们在发射火球的能力的cpp里用能力系统分配设置了这个键值对,所以能获取到)
	float Damage = 0.f;

	for (const auto& DamageType : MyGeContext->GetDamageTypes())
	{
		float ResistanceValue = 0;
		// 获得伤害类型对应的抗性标签
		FGameplayTag* ResistanceAttributeTag = FRPGAuraGameplayTags::Get().DamageTypesToResistancesMap.Find(DamageType);
		if (!ResistanceAttributeTag) { continue; }

		// 用抗性标签获取要捕获的属性
		const auto GameplayEnum = FRPGAuraGameplayTags::FindEnumByTag(*ResistanceAttributeTag);
		if (GameplayEnum)
		{
			switch (*GameplayEnum)
			{
			case EGameplayTagNum::FireResistance:
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.FireResistanceDef,
				                                                           EvaluateParameters,
				                                                           ResistanceValue);
				break;
			case EGameplayTagNum::PhysicalResistance:
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.PhysicalResistanceDef,
				                                                           EvaluateParameters,
				                                                           ResistanceValue);

				break;
			case EGameplayTagNum::LightningResistance:
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.LightingResistanceDef,
				                                                           EvaluateParameters,
				                                                           ResistanceValue);
				break;
			case EGameplayTagNum::ArcaneResistance:
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.ArcaneResistanceDef,
				                                                           EvaluateParameters,
				                                                           ResistanceValue);
				break;

			default: ;
			}
		}

		ResistanceValue = FMath::Clamp(ResistanceValue, 0.f, 100.f);
		// UE_LOG(UExecCalcDamageLog,Error,TEXT("物理抗性:[%.2f]"),ResistanceValue);
		// 有多种伤害类型,那么伤害会叠加
		Damage += ((GeSpec.GetSetByCallerMagnitude(DamageType) + SourceIntelligence * .25f) * ((100.f - ResistanceValue)
			/ 100.f));

		// 计算触发DeBuff的几率
		float DeBuffChance = GeSpec.GetSetByCallerMagnitude(
			FRPGAuraGameplayTags::Get().Abilities_DeBuff_Effects_Chance);
		DeBuffChance = DeBuffChance * FMath::Max(0.f, FMath::Abs((100.f - ResistanceValue))) / 100.f;
		if (DeBuffChance == 0.f) { continue; }
		const auto SuccessfulDeBuff = FMath::RandRange(1, 100) < DeBuffChance;
		if (const auto FindDeBuffTag = FRPGAuraGameplayTags::Get().DamageTypesToDeBuffMap.Find(DamageType);
			SuccessfulDeBuff && FindDeBuffTag)
		{
			FDeBuffInfo TmpDeBuffInfo = FDeBuffInfo();
			URPGAuraBlueprintFunctionLibrary::FillDeBuffInfoFromGeSpec(
				DamageType, *FindDeBuffTag, TmpDeBuffInfo, GeSpec);
			MyGeContext->AddDeBuffInfo(TmpDeBuffInfo);
		}
	}


	// 格挡几率
	float TargetBlockChance = 0.f;
	// 捕获目标格挡几率
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.BlockChanceDef,
	                                                           EvaluateParameters, TargetBlockChance);


	// 护甲穿透率
	float SourceArmorPenetration = SourceAs->GetArmorPenetration();
	SourceArmorPenetration = FMath::Clamp(SourceArmorPenetration * ArmorPenetrationFactor, 0, 100);

	// 有效护甲计算
	float EffectiveArmor = TargetArmor * ((100 - SourceArmorPenetration)
		/ 100.f);
	EffectiveArmor = FMath::Max<float>(EffectiveArmor, 0.f);

	// 目标暴击抵抗率
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.CriticalHitResistanceDef,
	                                                           EvaluateParameters, TargetCriticalHitResistance);


	// 源的暴击率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.CriticalHitChanceDef,
	                                                           EvaluateParameters, SourceCriticalHitChance);
	// 源的有效暴击几率
	float SourceEffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance;
	SourceEffectiveCriticalHitChance = FMath::Max<float>(SourceEffectiveCriticalHitChance, 0.f);

	// 捕获源的暴击伤害
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureAttributes.CriticalHitDamageDef,
	                                                           EvaluateParameters, SourceCriticalHitDamage);

	// 一点(有效护甲* EffectiveArmorFactor)抵扣一点伤害
	EffectiveArmor = FMath::Clamp(EffectiveArmor * EffectiveArmorFactor, 0, 100);

	Damage *= (100 - EffectiveArmor) / 100.f;
	// 计算是否暴击
	const auto bIsCriticalHit = (FMath::RandRange(1, 100) <= SourceEffectiveCriticalHitChance);
	Damage = bIsCriticalHit
		         ? Damage * (2.f) + SourceCriticalHitDamage
		         : Damage;

	// 根据格挡几率判断是否格挡
	const auto bIsBlock = (FMath::RandRange(1, 100) <= TargetBlockChance);
	Damage = bIsBlock ? Damage * (0.5f) : Damage;

	const auto bIsKnockBack = (FMath::RandRange(1, 100) <= GeSpec.GetSetByCallerMagnitude(
		FRPGAuraGameplayTags::Get().Abilities_SideEffect_KnockBack_Chance));

	MyGeContext->SetBIsBlockedHit(bIsBlock);
	MyGeContext->SetBIsCriticalHit(bIsCriticalHit);
	MyGeContext->SetIsKnockBackHit(bIsKnockBack);

	// UE_LOG(UExecCalcDamageLog, Warning,
	//        TEXT(
	// 	       "[Aura护甲穿透因子]:%.2f , "
	// 	       "[Aura护甲穿透率]:%.2f ,"
	// 	       "[Aura暴击几率]:%.2f ,"
	// 	       "[Aura有效暴击几率]:%.2f ,"
	// 	       "[Aura有效暴击伤害]:%.2f ,"
	// 	       "[敌方有效护甲因子]:%.2f ,"
	// 	       "[敌方护甲]:%.2f ,"
	// 	       "[敌方最终有效护甲]:%.2f ,"
	// 	       "[敌方抵抗暴击几率]:%.2f ,"
	// 	       "[敌方格挡几率]:%.2f ,"
	// 	       "[最终伤害]: %.2f"
	//        ), ArmorPenetrationFactor, SourceArmorPenetration, SourceCriticalHitChance, SourceEffectiveCriticalHitChance,
	//        SourceCriticalHitDamage,
	//        EffectiveArmorFactor,
	//        TargetArmor,
	//        EffectiveArmor, TargetCriticalHitResistance, TargetBlockChance, Damage);


	// 修改属性集中名为InComingDamage的属性值
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetInComingDamageAttribute(),
	                                                   EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
