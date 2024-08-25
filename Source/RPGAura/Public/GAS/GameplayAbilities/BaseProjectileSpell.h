// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "BaseDamageAbility.h"
#include "BaseProjectileSpell.generated.h"

class ABaseProjectile;
/**
 * 基础投射魔法能力类
 */
UCLASS()
class RPGAURA_API UBaseProjectileSpell : public UBaseDamageAbility
{
	GENERATED_BODY()

public:
	/// 生成投射物
	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnProjectile(const FHitResult&  HitResult) ;

protected:
	// 飞弹实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Class")
	TSubclassOf<ABaseProjectile> ProjectileClass;

	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
};
