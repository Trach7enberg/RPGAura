// Copyright GGBAO 


#include "GAS/Data/TagToAbilityInfoAsset.h"

#include "CoreTypes/RPGAuraCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(UTagToAbilityInfoAssetLog, All, All);

FTagToAbilityInfo UTagToAbilityInfoAsset::FindAbilityInfoByAbilityTag(const FGameplayTag& Tag, const bool bLogNotFound) const
{
	for (const FTagToAbilityInfo& AbilityInfo : AbilityInfos)
	{
		if (AbilityInfo.AbilityTag == Tag) { return AbilityInfo; }
	}

	if(bLogNotFound)
	{
		UE_LOG(UTagToAbilityInfoAssetLog,Warning,TEXT("[%s]无法找到标签为[%s]的技能信息"),*GetNameSafe(this),*Tag.ToString());
	}
	return FTagToAbilityInfo();
}
