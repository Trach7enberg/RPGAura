// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AbilityDescriptionAsset.generated.h"

struct FGameplayTag;
struct FAbilityDescription;
/**
 * 对应技能能力详细信息的数据资产
 */
UCLASS()
class RPGAURA_API UAbilityDescriptionAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/// 技能详细描述数组
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="AbilityDescription")
	TArray<FAbilityDescription> AbilityDescriptions;

	/// 根据能力标签查找对应技能的详细描述信息
	/// @param AbilityTag 
	/// @return 
	FAbilityDescription FindDescriptionByAbilityTag(const FGameplayTag& AbilityTag);
};
