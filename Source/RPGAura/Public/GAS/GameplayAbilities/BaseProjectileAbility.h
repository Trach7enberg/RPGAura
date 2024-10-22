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

	/// 生成多重投射物,(投射物有追踪效果) TODO 待修改生成多个投射物的击退次数应该只会触发一次 
	/// @param HitResult 
	/// @param SocketAssociatedWithMontageTag
	/// @param HomingActor
	/// @param bOverridePitch
	/// @param OverridePitch 
	UFUNCTION(BlueprintCallable, Category="Spawn")
	void SpawnProjectiles(const FHitResult& HitResult, const FGameplayTag SocketAssociatedWithMontageTag,
	                      AActor* HomingActor, bool bOverridePitch, float OverridePitch);

protected:
	// 生成多重射弹时的散射度
	UPROPERTY(EditDefaultsOnly, Category="Spawn", meta=(ClampMin="0", ClampMax="360"))
	float SpawnSpread = 90.f;

	// 当前能力能够生成的最大射弹数
	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	int32 MaxProjectileNum = 5;

	// 射弹是否追踪
	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	bool bIsHomingProjectile = false;

	// 射弹是否启用重力
	UPROPERTY(EditDefaultsOnly, Category="Spawn", meta=(ClampMin="0", ClampMax="10"))
	float ProjectileGravity = 0.f;

	// 追踪射弹的最小加速度
	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	float HomingMinAcceleration = 1600.f;

	// 追踪射弹的最大加速度
	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	float HomingMaxAcceleration = 3200.f;

	// 飞弹实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Class")
	TSubclassOf<ABaseProjectile> ProjectileClass;


	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel) override;

	/// 获取当前有效射弹数
	/// @return 
	virtual int32 GetValidProjectileNum(int32 AbilityLevel);
private:

	/// 创建生成飞弹实例
	/// @param Transform 
	/// @param Instigator 
	/// @return 
	ABaseProjectile* CreateProjectile(const FTransform& Transform, APawn* Instigator) const;

	UPROPERTY()
	TObjectPtr<USceneComponent> NonHomingTarget;
};
