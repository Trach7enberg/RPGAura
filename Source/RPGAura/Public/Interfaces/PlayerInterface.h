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

	/// 获取可分配的属性点数
	/// @return 
	virtual int32 GetAttributePointsReward(int32 CharacterLevel) = 0;

	
	virtual int32 GetSpellPointsReward(int32 CharacterLevel) = 0;

	// TODO
	virtual void AddToSpellPoints(int32 Points) = 0;

	// TODO 
	virtual void AddToAttributesPoints(int32 Points) = 0;
};
