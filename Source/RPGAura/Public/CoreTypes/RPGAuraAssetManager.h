// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "RPGAuraAssetManager.generated.h"

/**
 * 当前项目的AssetManager
 */
UCLASS()
class RPGAURA_API URPGAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static URPGAuraAssetManager& Get();

	/// 开始为游戏加载初始资产的时候就会调用这个函数,在这里初始化我们自己的GameplayTags
	virtual void StartInitialLoading() override;
};
