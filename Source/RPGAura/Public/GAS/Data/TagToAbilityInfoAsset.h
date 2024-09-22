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
	// 主动技能信息结构体数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FTagToAbilityInfo> AbilityInfosOffensive;

	// 被动技能信息结构体数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<FTagToAbilityInfo> AbilityInfosPassive;

	FTagToAbilityInfo FindOffensiveAbilityInfo(const FGameplayTag& Tag, bool bLogNotFound = true) const;

	/// 通过给定的标签查找与该标签Match的AbilityInfo
	FTagToAbilityInfo FindPassiveAbilityInfo(const FGameplayTag& Tag, bool bLogNotFound = true) const;

};
