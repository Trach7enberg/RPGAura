// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 和玩家相关的接口
 */
class RPGAURA_API IPlayerInterface
{
	GENERATED_BODY()

public:
	/// 添加玩家经验
	/// @param AddedXp 
	virtual void AddToPlayerXP(const int32 AddedXp) = 0;

	/// 获取玩家当前的经验值
	/// @return 
	virtual int32 GetPlayerCurrentXP() = 0;

	/// 玩家当前是否能升级
	/// @return 
	virtual bool CanBeLevelUp() = 0;

	/// 玩家升级
	virtual void LevelUp() = 0;

	/// 获取对应等级的属性点奖励
	/// @return 
	virtual int32 GetAttributePointsReward(int32 CharacterLevel) = 0;

	/// 获取对应等级的法术点奖励
	/// @param CharacterLevel 
	/// @return 
	virtual int32 GetSpellPointsReward(int32 CharacterLevel) = 0;

	/// 设置当前角色的可分配法术点
	/// @param Points 
	virtual void AddToSpellPoints(int32 Points) = 0;

	/// 设置当前角色的可分配属性点
	/// @param Points 
	virtual void AddToAttributesPoints(int32 Points) = 0;

	/// 获取当前角色可分配的属性点
	/// @return 
	virtual int32 GetCurrentAssignableAttributePoints() = 0;

	/// 获取当前角色可分配的法术点
	/// @return 
	virtual int32 GetCurrentAssignableSpellPoints() = 0;

	/// 保存进度 
	/// @param CheckPointTag 
	UFUNCTION(BlueprintCallable)
	virtual void SaveProgress(const FName& CheckPointTag) = 0;

	/// 加载角色的信息进度(包括GAS的能力、属性等)
	virtual void LoadProgress() = 0;
};
