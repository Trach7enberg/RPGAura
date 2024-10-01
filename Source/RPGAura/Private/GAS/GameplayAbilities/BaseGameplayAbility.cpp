// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseGameplayAbility.h"

#include "CoreTypes/RPGAuraCoreTypes.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GAS/Data/AbilityDescriptionAsset.h"

UBaseGameplayAbility::UBaseGameplayAbility()
{
	
}

void UBaseGameplayAbility::UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel)
{
	
}


FAbilityDescription UBaseGameplayAbility::GetAbilityDescription(const FGameplayTag& AbilityTag,
                                                                const int32 AbilityLevel)
{
	// TODO 可以进行优化 需要时才查找数据资产,要立即响应蓝图更改就每次都查找
	CurrentAbilityDescription = URPGAuraBlueprintFunctionLibrary::GetAbilityDescriptionAsset()->
			FindDescriptionByAbilityTag(AbilityTag);

	// TODO 可以进行优化 只有第一次和能力升级时才更新技能描述
	UpdateAbilityDescription(AbilityTag,AbilityLevel);
	return CurrentAbilityDescription;
}




