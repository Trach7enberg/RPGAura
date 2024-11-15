// Copyright GGBAO 


#include "GAS/Data/LootTiers.h"

#include "CoreTypes/RPGAuraCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(ULootTiersLog, All, All);

void ULootTiers::GetLootItems(TArray<FLootItem>& OutItems)
{
	if(OutItems.Num()){OutItems.Reset();}
	for (auto LootItem : LootItems)
	{
		for (int i = 0; i < LootItem.MaxNumberToSpawn; ++i)
		{
			const auto Chance = FMath::FRandRange(1.f, 100.f) < LootItem.ChanceToSpawn;
			if (Chance) { OutItems.Add(LootItem);}
			
		}
	}
}
