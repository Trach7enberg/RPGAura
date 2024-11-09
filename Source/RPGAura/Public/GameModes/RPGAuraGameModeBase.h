// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGAuraGameModeBase.generated.h"

class ULoadScreenSave;
class UMVVMLoadSlotSwitcher;
class UTagToAbilityInfoAsset;
/**
 * 游戏模式类
 */
UCLASS()
class RPGAURA_API ARPGAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ARPGAuraGameModeBase();

public:
	/// 保存数据到指定的槽位
	/// @param LoadSlotSwitcherViewModel  
	/// @param SaveSlotIndex 要保存的槽位
	void SaveLoadSlotData(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel,int32 SaveSlotIndex);

	/// 根据存档的名字从指定槽位加载数据
	/// @param Name 存档的名字(一般为玩家的名字)
	/// @param SlotIndex 
	/// @return 
	ULoadScreenSave* GetLoadSlotData(const FString& Name,int32 SlotIndex) const;

	/// 根据存档的名字从指定槽位加载数据
	/// @param LoadSlotSwitcherViewModel 
	/// @return 
	ULoadScreenSave* GetLoadSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel) const;

	/// 删除指定槽位的存档
	/// @param LoadSlotSwitcherViewModel
	bool DeleteSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel);

protected:

	/// 初始地图,请在Map中选择
	UPROPERTY(EditDefaultsOnly,Category="Level")
	FString StartingMapName{};
	
	// 关卡
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Level")
	TMap<FString,TSoftObjectPtr<UWorld>> LevelMaps{};

	virtual void BeginPlay() override;
};
