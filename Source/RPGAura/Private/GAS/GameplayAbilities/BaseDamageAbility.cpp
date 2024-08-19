// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseDamageAbilityLog, All, All);

void UBaseDamageAbility::AssignTagSetByCallerMagnitudeWithDamageType(const FGameplayEffectSpecHandle& SpecHandle,
                                                                     const float AbilityLevel) const
{
	if (!DamageTypesMap.Num())
	{
		UE_LOG(UBaseDamageAbilityLog, Warning, TEXT("DamageTypesMap为空!"));
		return;
	}
	for (const auto& Pair : DamageTypesMap)
	{
		// 分配一个键值对给SetByCaller,键是游戏标签,值是设定的Magnitude,到时候在GE蓝图中选择我们分配的标签,该GE就会应用我们这里设定的Magnitude值
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,
		                                                              Pair.Key,
		                                                              Pair.Value.GetValueAtLevel(AbilityLevel));
	}
}
