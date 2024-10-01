// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseDamageAbility.h"
#include "BaseProjectileAbility.generated.h"

class ABaseProjectile;
/**
 * 基础投弹能力类
 */
UCLASS()
class RPGAURA_API UBaseProjectileAbility : public UBaseDamageAbility
{
	GENERATED_BODY()

public:
	UBaseProjectileAbility();
	/// 生成投射物
	/// @param HitResult 击中数据
	/// @param SocketAssociatedWithMontageTag 生成飞弹的初始位置Socket,该Socket与对应的游戏标签相关联
	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnProjectile(const FHitResult& HitResult, const FGameplayTag SocketAssociatedWithMontageTag);

protected:
	
	// 飞弹实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Class")
	TSubclassOf<ABaseProjectile> ProjectileClass;
	

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
								 const FGameplayAbilityActivationInfo ActivationInfo,
								 const FGameplayEventData* TriggerEventData) override;

	virtual void UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel) override;
	
};
