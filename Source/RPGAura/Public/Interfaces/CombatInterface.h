// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

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
	virtual FVector GetCombatSocketLocation() = 0;

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

	/// 只在服务器上调用角色死亡
	UFUNCTION(BlueprintCallable)
	virtual void Die() = 0;

	/// 在角色头顶显示伤害
	/// @param Damage 伤害
	UFUNCTION(NetMulticast, Reliable)
	virtual void ShowDamageNumber(float Damage) = 0;
};
