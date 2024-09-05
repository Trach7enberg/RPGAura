// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TagToAbilityInfoAsset.generated.h"

struct FGameplayTag;
struct FTagToAbilityInfo;
/**
 *  存储着技能与其对应的标签、UI数据的资产
 */
UCLASS()
class RPGAURA_API UTagToAbilityInfoAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// 技能信息结构体数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FTagToAbilityInfo> AbilityInfos;

	FTagToAbilityInfo FindAbilityInfoByAbilityTag(const FGameplayTag& Tag,bool bLogNotFound = true) const;
};
