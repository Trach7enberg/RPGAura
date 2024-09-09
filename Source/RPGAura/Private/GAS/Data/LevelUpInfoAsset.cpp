// Copyright GGBAO 


#include "GAS/Data/LevelUpInfoAsset.h"

#include "CoreTypes/RPGAuraCoreTypes.h"

ULevelUpInfoAsset::ULevelUpInfoAsset() {}

int32 ULevelUpInfoAsset::GetLevelFromXp(const int32 Xp)
{
	if (!LevelUpInfos.Num()) { return 1; }


	for (int i = 1; i <= LevelUpInfos.Num(); ++i) { if (LevelUpInfos[i].LeveRequirement >= Xp) { return i; } }

	return 1;
}

int32 ULevelUpInfoAsset::GetMinimumXpRequiredForLevel(const int32 Level)
{
	if (!LevelUpInfos.Num() || Level == 0 || Level >= LevelUpInfos.Num()) { return 0; }

	return LevelUpInfos[Level].LeveRequirement;
}

int32 ULevelUpInfoAsset::GetMaximumXPofLevel(const int Level)
{
	if (!LevelUpInfos.Num() || Level == 0 || Level > LevelUpInfos.Num()) { return 0; }

	if (!LevelUpInfos.IsValidIndex(Level + 1)) { return 0; }

	return LevelUpInfos[Level + 1].LeveRequirement - 1;
}

int32 ULevelUpInfoAsset::GetLevelCorrespondingToXP(ECharacterClass CharacterClass, const int32 CharacterCurrentXP,
                                                   const int32 CharacterCurrentLevel)
{
	if (CharacterCurrentLevel == 0) { return 1; }

	int32 Result = CharacterCurrentLevel;

	// 用角色当前经验值循环比较数据资产中每一个等级所需的经验值
	for (int i = CharacterCurrentLevel; i < LevelUpInfos.Num() && i + 1 < LevelUpInfos.Num(); ++i)
	{
		// 如果当前经验值大于下一个等级所需要的最低经验值,那么等级增加
		if (CharacterCurrentXP >= LevelUpInfos[i + 1].LeveRequirement) { ++Result; }
		else { break; }
	}

	return Result;
}
