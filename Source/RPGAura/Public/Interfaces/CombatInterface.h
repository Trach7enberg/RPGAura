// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

enum class ECharacterClass : uint8;
class UNiagaraSystem;
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

	virtual ECharacterClass GetCharacterClass() = 0;

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

	/// 获取角色的召唤动画
	/// @return 
	UFUNCTION(BlueprintCallable)
	virtual UAnimMontage* GetSummonAnim() =0;

	/// 获取当前角色已经有的召唤召唤物的数量
	UFUNCTION(BlueprintCallable)
	virtual int32 GetCurrentSummonsCount() =0;

	/// 自增角色当前的召唤物数
	UFUNCTION(BlueprintCallable)
	virtual void UpdateCurrentSummonsCount(int32 NewCount) =0;

	/// 获取角色能可以召唤的最大的召唤物的数量
	UFUNCTION(BlueprintCallable)
	virtual int32 GetMaxSummonsCount() =0;

	/// 只在服务器上调用角色死亡
	UFUNCTION(BlueprintCallable)
	virtual void Die() = 0;

	UFUNCTION(BlueprintCallable)
	virtual bool IsCharacterDie() = 0;

	/// 获取和当前角色正在战斗的目标角色
	/// @return 
	UFUNCTION(BlueprintCallable)
	virtual AActor* GetCombatTarget() =0;

	/// 设置暂存当前角色正在战斗的目标角色
	/// @param CombatTarget 
	UFUNCTION(BlueprintCallable)
	virtual void SetCombatTarget(AActor* CombatTarget) =0;


	/// 在角色头顶显示伤害  //TODO 只在本地客户端上显示伤害数值
	/// @param Damage 伤害
	/// @param bBlockedHit 是否格挡
	/// @param bCriticalHit 是否暴击
	UFUNCTION(NetMulticast, Reliable)
	virtual void ShowDamageNumber(const float Damage, bool bBlockedHit = false, bool bCriticalHit = false) = 0;

	/// 获取当前人物身上的血液效果
	/// @return 
	UFUNCTION(BlueprintCallable)
	virtual UNiagaraSystem* GetBloodEffect() = 0;

	/// 播放角色被召唤的动画
	UFUNCTION(BlueprintCallable)
	virtual void StartSummonAnim() = 0;

	/// 显示角色触发DeBuff的特效
	/// @param DeBuffType 负面标签
	virtual void ShowDeBuffVfx(FGameplayTag DeBuffType) = 0;

	/// 添加冲击力(当死亡时)
	/// @param Impulse 
	virtual void AddDeathImpulse(const FVector& Impulse) = 0;

	/// 给角色添加击退
	/// @param Direction 击退的方向
	virtual void AddKnockBack(const FVector& Direction) = 0;

	/// 设置当前角色是否正在释放电击技能
	/// @param Enabled
	UFUNCTION(BlueprintCallable)
	virtual void SetCastShockAnimState(const bool Enabled) = 0;

	/// 获取当前角色是否正在释放电击技能的状态值
	/// @return
	UFUNCTION(BlueprintCallable)
	virtual bool GetCastShockAnimState() = 0;

	/// 获取当前角色的武器,可能为nullptr
	/// @return 
	UFUNCTION(BlueprintCallable)
	virtual USkeletalMeshComponent* GetWeaponMesh() = 0;
};
