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

public:
	/// 造成伤害(给对象应用GE)
	/// @param Suffer
	UFUNCTION(BlueprintCallable, Category="GameplayEffect")
	void CauseDamage(AActor* Suffer);
	
protected:
	// 伤害GE实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 当前能力所造成的伤害类型,GT映射到对应的曲线表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypesMap;

	/// 分配一个键值对给SetByCaller,并且将当前的伤害类型传递到GE上下中
	/// @param SpecHandle 
	/// @param AbilityLevel
	void AssignTagSetByCallerMagnitudeWithDamageType(const FGameplayEffectSpecHandle& SpecHandle,
	                                                 const float AbilityLevel) const;
};
