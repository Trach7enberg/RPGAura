// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BaseGameInstance.generated.h"

/**
 *  项目基础游戏实例类
 */
UCLASS()
class RPGAURA_API UBaseGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	/// 关卡中默认的playerStart的playerStartTag,用于生成在默认的位置上
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FName PlayerStartTag = FName("DefaultSpawn");

	/// 用于从磁盘加载数据的存档名
	UPROPERTY()
	FString LoadSlotName{};

	/// 用于从磁盘加载数据的存档索引
	UPROPERTY()
	int32 LoadSlotIndex{};
};
