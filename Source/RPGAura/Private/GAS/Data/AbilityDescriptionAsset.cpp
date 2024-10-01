// Copyright GGBAO 


#include "GAS/Data/AbilityDescriptionAsset.h"

#include "CoreTypes/RPGAuraCoreTypes.h"

FAbilityDescription UAbilityDescriptionAsset::FindDescriptionByAbilityTag(const FGameplayTag& AbilityTag)
{
	for (const auto& Description : AbilityDescriptions)
	{
		if (Description.AbilityTag == AbilityTag)
		{
			return Description;
		}
	}
	return FAbilityDescription();
}
