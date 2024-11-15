// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPGAuraGameModeBase.generated.h"

class USaveGame;
class UBaseGameInstance;
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
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	/// 保存数据到指定的槽位
	/// @param LoadSlotSwitcherViewModel  
	/// @param SaveSlotIndex 要保存的槽位
	bool SaveLoadSlotData(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel, int32 SaveSlotIndex);

	/// 覆盖已经存在的存档数据,没有则不进行任何操作
	/// @param SaveGameObj 
	bool SaveLoadSlotData(ULoadScreenSave* SaveGameObj);

	/// 保存世界的状态到当前关卡的存档中(如果有旅行目标的地图,则设置地图为目的地图)
	/// @param World 当前世界
	/// @param DestinationMapAssetName 旅行目标地图
	void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString(""));

	/// 从当前存档中恢复世界的状态
	/// @param World 
	void LoadWorldState(UWorld* World);

	/// 根据存档的名字从指定槽位加载数据
	/// @param Name 存档的名字(一般为玩家的名字)
	/// @param SlotIndex 
	/// @return 
	ULoadScreenSave* GetLoadSlotData(const FString& Name, int32 SlotIndex) const;

	/// 根据存档的名字从指定槽位加载数据
	/// @param LoadSlotSwitcherViewModel 
	/// @return 
	ULoadScreenSave* GetLoadSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel) const;

	/// 删除指定槽位的存档
	/// @param LoadSlotSwitcherViewModel
	bool DeleteSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel);

	/// 打开关卡
	/// @param LevelName 
	void OpenLevel(const FString& LevelName);

	/// 获取当前的存档(如游戏已经读取存档并开始,那么获取的就是这个存档)
	/// @return 
	ULoadScreenSave* GetCurrentInGameSaveData();

	/// 玩家死亡时,简单的重新回到当前关卡 
	/// @param DeadCharacter 
	void CharacterDie(ACharacter* DeadCharacter);

	/// 根据给定的关卡地图资产名字获取游戏模式中存储的关卡地图合集里某个地图所对应的名字
	/// @param MapAssetName 
	/// @return 
	FString GetLevelMapNameFromMapAssetName(const FString& MapAssetName) const;

	/// 获取字符串所对应的关卡的资产名字(即OpenLevel所需要的那个名字)
	/// @param KeyStr 
	/// @return 
	FString GetLevelMapAssetNameByString(const FString& KeyStr) const;

	FString GetStartingMapName() const { return StartingMapName; }
	FName   GetDefaultPlayerStartTag() const { return DefaultPlayerStartTag; }

	void SetDefaultPlayerStartTag(const FName& NewDefaultPlayerStartTag)
	{
		this->DefaultPlayerStartTag = NewDefaultPlayerStartTag;
	}

protected:
	/// 初始(默认)地图,请在Map中选择
	UPROPERTY(EditDefaultsOnly, Category="Level")
	FString StartingMapName{};

	/// 关卡默认的初始位置的标识
	UPROPERTY(EditDefaultsOnly, Category="Level")
	FName DefaultPlayerStartTag{};

	// 关卡
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Level")
	TMap<FString, TSoftObjectPtr<UWorld>> LevelMaps{};

	UBaseGameInstance* GetMyGameInstance();

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UBaseGameInstance> MyGameInstance;
};
