// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

/// 和战斗相关的接口
UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
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
};
