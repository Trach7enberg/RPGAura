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
	/// 检测地面上的点,用于生成奥术法术 
	/// @param StartCenter    
	UFUNCTION(BlueprintCallable)
	void DetectGroundPoints(const FVector& StartCenter);

	/// 生成奥术碎片,完成之后会自动结束能力
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
	/// 由Arcane定时器调用的回调函数
	UFUNCTION()
	void SpawnArcaneWithDelay();

	/// 显示调式信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane")
	bool BShowDebugDraw = false;

	// 奥术法术的半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane")
	float ArcaneOutRadius = 210.f;

	// 奥术法术的内半径
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane")
	float ArcaneInnerRadius = 125.f;

	// 奥术法术的最大半径的容差, eg: 外半径为200,容差为30,那么当Actor距圆心230时仍然能受到伤害 (不是一定会生效,因为前提是要被范围检测到)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane", meta=(ClampMin=10.f, ClampMax=100.f))
	float ArcaneOutRadiusTolerance = 30.f;

	// 奥术法术的分散角度(以冲击点为圆心)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane")
	float ArcaneSpread = 360.f;

	// 奥术法术的生成数量 (由技能等级决定)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane")
	int32 ArcaneSpawnNum = 5.f;

	// 奥术法术生成延迟秒数(如果有多个)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane", meta=(ClampMin=0.1f, ClampMax=1.0f))
	float ArcaneSpawnDelay = .2f;

	// 刚好处于外圆时的最低伤害衰减系数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Arcane", meta=(ClampMin=0.2f, ClampMax=1.0f))
	float ArcaneOutRadiusMinDamageFactor = .5f;

	/// 鼠标指针的碰撞点(在蓝图中设置)
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Arcane")
	FVector CursorImpactPoint{};

	/// 以地面坐标数组中的点为指定中心半径生成奥术碎片并且给检测到的Actor应用GE
	/// @param CurrentCenter 
	virtual void CauseArcaneRadiusDamage(const FVector& CurrentCenter);

	virtual int32 GetValidAbilityCount(const int32 AbilityLevel) override;

private:
	UPROPERTY()
	int32 LocalPointsIndex = 0;

	UPROPERTY()
	TArray<FVector> GroundPoints;

	// 用于生成多个奥术特效的定时器
	UPROPERTY()
	FTimerHandle ArcaneTimeHandle;
	
};
