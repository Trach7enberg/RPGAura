// Copyright GGBAO 


#include "GAS/Data/PickupMessageAsset.h"

#include "CoreTypes/RPGAuraCoreTypes.h"

FUIWidgetRow UPickupMessageAsset::FindPickupMessageByTag(const FGameplayTag& Tag)
{
	for (const auto& Element : PickupMessage) { if (Element.Tag == Tag) { return Element; } }
	return FUIWidgetRow();
}
