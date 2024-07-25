// Copyright GGBAO 


#include "GAS/MMC/MMCMaxMana.h"

#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMCMaxMana::UMMCMaxMana()
{
	CaptureAttributeDef.AttributeToCapture = UBaseAttributeSet::GetIntelligenceAttribute();

	// 设置属性的来源
	CaptureAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;

	CaptureAttributeDef.bSnapshot = false;

	// 将捕获的属性添加到捕获列表,确保我们后面的函数能够找到这个属性以用于计算
	RelevantAttributesToCapture.Add(CaptureAttributeDef);
}

float UMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const
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
		return 0;
	}

	const auto CaptureCharacterLevel = TargetActorCombatInterface->GetCharacterLevel();


	return CaptureCharacterLevel * 10.f + CaptureValue * 2.5f + BaseValue;
}
