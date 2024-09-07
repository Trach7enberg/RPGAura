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
	if (!LevelUpInfos.Num() || Level == 0 || Level > LevelUpInfos.Num()) { return 0; }

	return LevelUpInfos[Level].LeveRequirement;
}

int32 ULevelUpInfoAsset::GetMaximumXPofLevel(const int Level)
{
	if (!LevelUpInfos.Num() || Level == 0 || Level > LevelUpInfos.Num()) { return 0; }

	if (!LevelUpInfos.IsValidIndex(Level + 1)) { return 0; }

	return LevelUpInfos[Level + 1].LeveRequirement - 1;
}
