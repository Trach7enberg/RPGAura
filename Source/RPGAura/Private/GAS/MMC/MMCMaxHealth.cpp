// Copyright GGBAO 


#include "GAS/MMC/MMCMaxHealth.h"

#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "Interfaces/CombatInterface.h"

DEFINE_LOG_CATEGORY_STATIC(UMMCMaxHealthLog,All,All);

UMMCMaxHealth::UMMCMaxHealth()
{
	// 设置要捕获的属性,从属性集(AttributeSet)的静态方法中获得
	CaptureAttributeDef.AttributeToCapture = UBaseAttributeSet::GetVigorAttribute();

	// 设置属性的来源
	CaptureAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	// 设置快照,快照决定我们什么时候捕获属性,是在GE蓝图类的GamePlayEffectSpec创建时还是在应用时?
	// 这里暂时用不到,因为我们创建了GamePlayEffectSpec后就会立即应用GE来对初始化次级属性,所有这不重要
	// 再比如一个在空中飞着的火球,快照是在创建了这个火球的GamePlayEffectSpec时候立马就捕获设置一个属性,但这个不是真实的值
	// 不设置快照属性就是在应用效果的时候才会捕捉对方的属性,然后以此进行计算
	CaptureAttributeDef.bSnapshot = false;

	// 将捕获的属性添加到捕获列表,确保我们后面的函数能够找到这个属性以用于计算
	RelevantAttributesToCapture.Add(CaptureAttributeDef);
}

float UMMCMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const
{
	// const FGameplayEffectSpec &Spec包含当前GE蓝图类里的所有信息数据,谁用了当前这个MMC,那么Spec就是谁的GE数据

	// 从捕获的源或者目标中获取Tag 
	const auto SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	// 获取捕获的属性的值
	float CaptureValue = 0.f;
	GetCapturedAttributeMagnitude(CaptureAttributeDef, Spec, EvaluateParameters, CaptureValue);

	
	const auto TargetActorCombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	if (!TargetActorCombatInterface)
	{
		UE_LOG(UMMCMaxHealthLog,Warning,TEXT("获取ICombatInterface接口失败!!"));
		return 0;
	}

	const auto CaptureCharacterLevel = TargetActorCombatInterface->GetCharacterLevel();


	return CaptureCharacterLevel * 10.f + CaptureValue * 2.5f + BaseValue;
}
