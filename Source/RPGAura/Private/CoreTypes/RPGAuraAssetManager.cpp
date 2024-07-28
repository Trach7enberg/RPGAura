// Copyright GGBAO 


#include "CoreTypes/RPGAuraAssetManager.h"

#include "CoreTypes/RPGAuraGameplayTags.h"

URPGAuraAssetManager &URPGAuraAssetManager::Get()
{
	check(GEngine);

	return *(Cast<URPGAuraAssetManager>(GEngine->AssetManager));
}

void URPGAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FRPGAuraGameplayTags::InitGameplayTags();
}
