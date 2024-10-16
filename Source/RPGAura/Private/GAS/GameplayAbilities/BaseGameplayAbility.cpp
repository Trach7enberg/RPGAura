// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseGameplayAbility.h"

#include "CoreTypes/RPGAuraCoreTypes.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/AbilityDescriptionAsset.h"

UBaseGameplayAbility::UBaseGameplayAbility() {}

void UBaseGameplayAbility::UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel) {}

float UBaseGameplayAbility::GetManaCost(const float InLevel) const
{
	if (!GetCostGameplayEffect()) { return 0.f; }

	float Result = 0.f;
	// 遍历CostGE的modifier数组
	for (const auto& Modifier : GetCostGameplayEffect()->Modifiers)
	{
		if (Modifier.Attribute == UBaseAttributeSet::GetCurrentManaAttribute())
		{
			// 获取静态的Scalable cost值
			Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, Result);
		}
	}

	return Result;
}

float UBaseGameplayAbility::GetCoolDown(const float InLevel) const
{
	if (!GetCooldownGameplayEffect()) { return 0.f; }

	float Result = 0.f;
	GetCooldownGameplayEffect()->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Result);

	return Result;
}


FAbilityDescription UBaseGameplayAbility::GetAbilityDescription(const FGameplayTag& AbilityTag,
                                                                const int32 AbilityLevel)
{
	// TODO 可以进行优化 需要时才查找数据资产,要立即响应蓝图更改就每次都查找
	CurrentAbilityDescription = URPGAuraBlueprintFunctionLibrary::GetAbilityDescriptionAsset(this)->
		FindDescriptionByAbilityTag(AbilityTag);

	// TODO 可以进行优化 只有第一次和能力升级时才更新技能描述 
	UpdateAbilityDescription(AbilityTag, AbilityLevel);
	return CurrentAbilityDescription;
}
