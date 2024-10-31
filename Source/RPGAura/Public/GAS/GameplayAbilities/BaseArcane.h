// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseDamageAbility.h"
#include "BaseArcane.generated.h"

class UNiagaraSystem;
/**
 * 基础奥术法术能力类
 */
UCLASS()
class RPGAURA_API UBaseArcane : public UBaseDamageAbility
{
	GENERATED_BODY()

public:
	/// 
	/// @param StartCenter 
	/// @param Radius 
	/// @param Spread 
	/// @param Points 
	UFUNCTION(BlueprintCallable)
	void DetectGroundPoints(const FVector& StartCenter, const float Radius, const float Spread, const int32 Points);

	/// 生成奥术碎片
	/// @param SpawnCenter 
	UFUNCTION(BlueprintCallable)
	void SpawnArcaneShards(const FVector& SpawnCenter);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                        const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
	                        bool bWasCancelled) override;

protected:
	/// 
	UFUNCTION()
	void SpawnArcaneWithDelay();

private:
	UPROPERTY()
	int32 LocalPointsIndex = 0;
	
	UPROPERTY()
	TArray<FVector> GroundPoints;

	// 用于生成多个奥术特效的定时器
	UPROPERTY()
	FTimerHandle ArcaneTimeHandle;
};
