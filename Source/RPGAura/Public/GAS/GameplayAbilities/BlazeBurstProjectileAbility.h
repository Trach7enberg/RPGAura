// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseProjectileAbility.h"
#include "BlazeBurstProjectileAbility.generated.h"

/**
 * 焰爆扩散技能
 */
UCLASS()
class RPGAURA_API UBlazeBurstProjectileAbility : public UBaseProjectileAbility
{
	GENERATED_BODY()

public:
	/// 
	/// @param Center
	/// @param bOverridePitch 
	/// @param OverridePitch 
	UFUNCTION(BlueprintCallable, Category="Spawn")
	virtual void SpawnProjectilesAround(const FVector& Center, const bool bOverridePitch, const float OverridePitch);

protected:
	// 技能启动时的冲击半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawn")
	float AroundRadius = 200.f;

	/// 飞弹散开的速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawn")
	float AroundBurstSpeed = 800.f;
	
	// 飞弹的跟随延迟
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Spawn", meta=(ClampMin=0.1f, ClampMax=1.f))
	float FollowDelay = .5f;

	/// 从给定的中心对在范围内的敌人添加冲击  TODO 需要提取到父类公共函数? 
	/// @param Center
	/// @param Radius 
	virtual void SpawnImpulsiveAtLoc(const FVector& Center, const float Radius);
	virtual int32 GetValidAbilityCount(const int32 AbilityLevel) override;
};
