// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfoAsset.generated.h"

struct FLevelUpInfoStruct;
/**
 *  用于角色升级信息的数据资产
 */
UCLASS()
class RPGAURA_API ULevelUpInfoAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	ULevelUpInfoAsset();

	/// 等级信息结构数据,存储的等级所需经验值从小到大递增且索引0为占位符
	/// 即LevelUpInfos[1] = 等级1的信息 , LevelUpInfos[2] = 等级2的信息
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TArray<FLevelUpInfoStruct> LevelUpInfos;

	/// 从给定的经验值获取相应的等级
	/// @param Xp 
	/// @return 
	int32 GetLevelFromXp(int32 Xp);

	/// 获取升级到给定的等级所需要的最小经验数
	/// @param Level 
	/// @return 
	int32 GetMinimumXpRequiredForLevel(int32 Level);

	/// 获取给定等级能到达的最大经验值数
	/// @param Level 
	/// @return 
	int32 GetMaximumXPofLevel(int Level);
};
