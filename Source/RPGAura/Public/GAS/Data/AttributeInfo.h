// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

/// 用于广播GAS属性的结构体
USTRUCT(BlueprintType)
struct FRPGAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;

};

/**
 * 存储着游戏标签到属性的映射数据资产表
 */
UCLASS()
class RPGAURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	/// 通过GT返回对应的属性信息
	/// @param AttributeTag 属性标签
	/// @param BLogNotFound 没找到时是否打印日志
	/// @return 属性信息结构体
	FRPGAuraAttributeInfo GetAttributeInfoByTag(const FGameplayTag &AttributeTag, bool BLogNotFound = true) const;

	/// 在蓝图中填充的数据数组,游戏标签到属性的映射
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRPGAuraAttributeInfo> RPGAuraAttributeInfos;
};
