// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTiers.generated.h"

struct FLootItem;
/**
 * 战利品数据表
 */
UCLASS()
class RPGAURA_API ULootTiers : public UDataAsset
{
	GENERATED_BODY()

public:
	/// 获得经过计算的最终的战利品数量数组
	/// @param OutItems 
	void GetLootItems(TArray<FLootItem>& OutItems);
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FLootItem> LootItems{};
};
