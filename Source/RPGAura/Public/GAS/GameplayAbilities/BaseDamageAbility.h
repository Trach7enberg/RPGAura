// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "BaseDamageAbility.generated.h"

/**
 *  基础伤害类型的能力
 */
UCLASS()
class RPGAURA_API UBaseDamageAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

protected:
	
	// 伤害GE实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 伤害类型,GT映射到对应的曲线表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TMap<FGameplayTag,FScalableFloat> DamageTypesMap;

	/// 分配一个键值对给SetByCaller
	/// @param SpecHandle 
	/// @param AbilityLevel 
	void AssignTagSetByCallerMagnitudeWithDamageType(const FGameplayEffectSpecHandle &SpecHandle,float AbilityLevel) const;
};
