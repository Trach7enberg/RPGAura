// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

/*
 * 用于处理存档点的接口
 */
UINTERFACE(MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPGAURA_API ISaveInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/// 是否加载(更改)Transform
	/// @return 
	virtual bool ShouldLoadTransform() = 0;

	/// 加载已经保存的信息
	virtual void LoadActorSaveInfo() = 0;

	/// 返回当前存档点位置的移动目的地
	/// @param OutDestination 
	virtual void GetMoveToLocation(FVector& OutDestination) = 0;
};
