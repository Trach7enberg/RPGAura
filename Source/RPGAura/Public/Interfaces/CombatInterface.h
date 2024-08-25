// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

struct FGameplayTag;
struct FMontageWithTag;
class UAnimMontage;
/// 和战斗相关的接口
UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 战斗接口类
 */
class RPGAURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	/// 获取当前角色的等级
	/// @return 等级
	virtual int32 GetCharacterLevel() = 0;

	/// 获取武器攻击的socket位置(比如子弹、火球发射的位置)
	/// @return
	UFUNCTION(BlueprintCallable)
	virtual FVector GetCombatSocketLocation(const FGameplayTag& GameplayTag) = 0;

	/// 使角色朝向目标位置
	/// @param TargetLoc 目标位置
	UFUNCTION(BlueprintCallable)
	virtual void UpdateCharacterFacingTarget(const FVector& TargetLoc) = 0;

	/// 获取受击蒙太奇动画
	/// @return 受击蒙太奇动画
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetHitReactAnim() =0;

	///  获取死亡蒙太奇动画
	/// @return 死亡蒙太奇动画
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetDeathAnim() =0;

	UFUNCTION(BlueprintCallable)
	virtual TArray<FMontageWithTag> GetAttackAnims() =0;
	
	/// 只在服务器上调用角色死亡
	UFUNCTION(BlueprintCallable)
	virtual void Die() = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsCharacterDie() = 0;

	/// 在角色头顶显示伤害  //TODO 只在本地客户端上显示伤害数值
	/// @param Damage 伤害
	/// @param bBlockedHit 是否格挡
	/// @param bCriticalHit 是否暴击
	UFUNCTION(NetMulticast, Reliable)
	virtual void ShowDamageNumber(const float Damage, bool bBlockedHit = false, bool bCriticalHit = false) = 0;
};
