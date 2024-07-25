// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

/// 
UINTERFACE(MinimalAPI)
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
};
