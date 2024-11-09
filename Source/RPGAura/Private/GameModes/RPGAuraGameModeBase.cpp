// Copyright GGBAO 


#include "GameModes/RPGAuraGameModeBase.h"

#include "Characters/AuraCharacter.h"
#include "Controller/BasePlayerController.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "Kismet/GameplayStatics.h"
#include "Save/LoadScreen/LoadScreenSave.h"
#include "UI/MVVM/ViewModel/MVVMLoadSlotSwitcher.h"

DEFINE_LOG_CATEGORY_STATIC(ARPGAuraGameModeBaseLog, All, All);

ARPGAuraGameModeBase::ARPGAuraGameModeBase()
{
	DefaultPawnClass = AAuraCharacter::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
}

void ARPGAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	const auto Level = LevelMaps.Find(StartingMapName);
	if (!Level)
	{
		UE_LOG(ARPGAuraGameModeBaseLog, Error, TEXT("[%s]无法找到关卡!"), *FString::Printf(TEXT(__FUNCTION__)));
	}
}


void ARPGAuraGameModeBase::SaveLoadSlotData(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel, const int32 SaveSlotIndex)
{
	if (!LoadSlotSwitcherViewModel) { return; }
	
	// 要保持的存档如果已经存在,则删除
	DeleteSlotData(LoadSlotSwitcherViewModel);

	auto SaveGameObj = UGameplayStatics::CreateSaveGameObject(ULoadScreenSave::StaticClass());
	if (!SaveGameObj) { return; }

	auto LoadScreenSaveGameObj = Cast<ULoadScreenSave>(SaveGameObj);
	if (!LoadScreenSaveGameObj) { return; }

	LoadScreenSaveGameObj->PlayerName = LoadSlotSwitcherViewModel->GetPlayerName();
	LoadScreenSaveGameObj->CurrentLoadSlotSwitcherLoadSlotTag = FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot_Taken;
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGameObj, LoadSlotSwitcherViewModel->GetLoadSlotName(),
	                                 SaveSlotIndex);
}

ULoadScreenSave* ARPGAuraGameModeBase::GetLoadSlotData(const FString& Name, const int32 SlotIndex) const
{
	return Cast<ULoadScreenSave>(UGameplayStatics::LoadGameFromSlot(Name, SlotIndex));
}

ULoadScreenSave* ARPGAuraGameModeBase::GetLoadSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel) const
{
	if (!UGameplayStatics::DoesSaveGameExist(LoadSlotSwitcherViewModel->GetLoadSlotName(),
	                                         LoadSlotSwitcherViewModel->GetLoadSlotInSave())) { return nullptr; }
	return Cast<ULoadScreenSave>(UGameplayStatics::LoadGameFromSlot(LoadSlotSwitcherViewModel->GetLoadSlotName(),
	                                                                LoadSlotSwitcherViewModel->GetLoadSlotInSave()));
}

bool ARPGAuraGameModeBase::DeleteSlotData(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel)
{
	if (!IsValid(LoadSlotSwitcherViewModel)) { return false; }

	// 要保持的存档已经存在,则删除
	if (UGameplayStatics::DoesSaveGameExist(LoadSlotSwitcherViewModel->GetLoadSlotName(), LoadSlotSwitcherViewModel->GetLoadSlotInSave()))
	{
		return UGameplayStatics::DeleteGameInSlot(LoadSlotSwitcherViewModel->GetLoadSlotName(), LoadSlotSwitcherViewModel->GetLoadSlotInSave());
	}

	return false;
}

