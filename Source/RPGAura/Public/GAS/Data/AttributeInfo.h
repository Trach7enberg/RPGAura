// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"


/**
 * 存储着游戏标签到属性的映射数据资产表类
 */
UCLASS()
class RPGAURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UAttributeInfo();
	/// 通过GT返回对应的属性信息
	/// @param AttributeTag 属性标签
	/// @return 属性信息结构体,没找到则为nullptr
	FRPGAuraAttributeInfo FindAttributeInfoByTag(const FGameplayTag& AttributeTag);


	/// 在蓝图中填充的数据数组,游戏标签到属性的映射
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FRPGAuraAttributeInfo> RPGAuraAttributeInfos{};
};
