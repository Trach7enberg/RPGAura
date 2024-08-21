// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PickupMessageAsset.generated.h"

struct FGameplayTag;
struct FUIWidgetRow;

/**
 * 拾取信息数据资产表
 */
UCLASS()
class RPGAURA_API UPickupMessageAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/// 拾取信息表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FUIWidgetRow> PickupMessage;

	
	FUIWidgetRow FindPickupMessageByTag(const FGameplayTag& Tag);
};
