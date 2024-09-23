// Copyright GGBAO 


#include "GAS/Data/TagToAbilityInfoAsset.h"

#include "CoreTypes/RPGAuraCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(UTagToAbilityInfoAssetLog, All, All);

FTagToAbilityInfo UTagToAbilityInfoAsset::FindOffensiveAbilityInfo(const FGameplayTag& AbilityTag,
                                                                      const bool bLogNotFound) const
{
	for(int i = 0;AbilityTag.IsValid() && i<AbilityInfosOffensive.Num();++i)
	{
		if (AbilityInfosOffensive[i].AbilityTag == AbilityTag) { return AbilityInfosOffensive[i]; }
	}
	if (bLogNotFound)
	{
		UE_LOG(UTagToAbilityInfoAssetLog, Warning, TEXT("[%s]无法找到标签为[%s]的主动技能信息"), *GetNameSafe(this), *AbilityTag.ToString());
	}
	return FTagToAbilityInfo();
}

FTagToAbilityInfo UTagToAbilityInfoAsset::FindPassiveAbilityInfo(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for(int i = 0;AbilityTag.IsValid() && i<AbilityInfosPassive.Num();++i)
	{
		if (AbilityInfosPassive[i].AbilityTag == AbilityTag) { return AbilityInfosPassive[i]; }
	}

	if (bLogNotFound)
	{
		UE_LOG(UTagToAbilityInfoAssetLog, Warning, TEXT("[%s]无法找到标签为[%s]的被动技能信息"), *GetNameSafe(this), *AbilityTag.ToString());
	}
	return FTagToAbilityInfo();
}


