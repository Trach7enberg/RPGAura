// Copyright GGBAO 


#include "UI/MVVM/ViewModel/MVVMLoadScreen.h"

#include "GameplayTagContainer.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameInstance/BaseGameInstance.h"
#include "GameModes/RPGAuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Save/LoadScreen/LoadScreenSave.h"
#include "UI/MVVM/ViewModel/MVVMLoadSlotSwitcher.h"

DEFINE_LOG_CATEGORY_STATIC(UMVVMLoadScreenLog, All, All);

void UMVVMLoadScreen::InitialLoadSlotSwitcherWidgetViewModel()
{
	if (!UmvvmLoadSlotSwitcherClass)
	{
		UE_LOG(UMVVMLoadScreenLog, Error, TEXT("[%s]ViewModel Class 为Nullptr!"), *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}
	const auto Switcher1 = NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass);
	const auto Switcher2 = NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass);
	const auto Switcher3 = NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass);

	Switcher1->SetLoadSlotName(FString("Switcher1"));
	Switcher2->SetLoadSlotName(FString("Switcher2"));
	Switcher3->SetLoadSlotName(FString("Switcher3"));
	Switcher1->SetLoadSlotInSave(0);
	Switcher2->SetLoadSlotInSave(1);
	Switcher3->SetLoadSlotInSave(2);
	Switcher1->SetTestStr(FString(TEXT("新关卡")));
	Switcher2->SetTestStr(FString(TEXT("新关卡")));
	Switcher3->SetTestStr(FString(TEXT("新关卡")));

	// 每一个插槽切换器对应的ViewModel
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_1, Switcher1);
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_2, Switcher2);
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_3, Switcher3);
}

UMVVMLoadSlotSwitcher* UMVVMLoadScreen::GetSwitcherViewModelByTag(FGameplayTag LoadSlotSwitchWidgetTag)
{
	auto ViewModel = LoadSlotSwitcherViewModelMap.Find(LoadSlotSwitchWidgetTag);
	if (!ViewModel)
	{
		UE_LOG(UMVVMLoadScreenLog, Error, TEXT("[%s]无法获取ViewModel!"), *FString::Printf(TEXT(__FUNCTION__)));

		return nullptr;
	}
	return *ViewModel;
}

void UMVVMLoadScreen::NewSaveButtonPressed(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel, const FString& NewSaveName)
{
	if (!LoadSlotSwitcherViewModel) { return; }

	const auto GameMode = GetMyGameMode();
	const auto Gi       = GetMyGameInstance();
	if (!GameMode || !Gi) { return; }

	// 新建存档时就设置玩家等级、名字、地图名字、玩家初始位置用于持久化
	LoadSlotSwitcherViewModel->SetPlayerLevel(LoadSlotSwitcherViewModel->GetPlayerLevel());
	LoadSlotSwitcherViewModel->SetPlayerName(NewSaveName);
	LoadSlotSwitcherViewModel->SetMapName(GameMode->GetStartingMapName());
	LoadSlotSwitcherViewModel->SetPlayerStartTag(GameMode->GetDefaultPlayerStartTag());

	GameMode->SaveLoadSlotData(LoadSlotSwitcherViewModel, LoadSlotSwitcherViewModel->GetLoadSlotInSave());

	LoadSlotSwitcherViewModel->BroadcastSwitcherLoadSlotChange(FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot_Taken);
}

void UMVVMLoadScreen::NewGameButtonPressed(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel)
{
	if (!LoadSlotSwitcherViewModel) { return; }

	LoadSlotSwitcherViewModel->BroadcastSwitcherLoadSlotChange(FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot_Enter);
}

void UMVVMLoadScreen::SelectLoadButtonPressed(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel)
{
	const auto Gi = GetMyGameInstance();
	if (!LoadSlotSwitcherViewModel || !Gi) { return; }

	OnSlotSelected.Broadcast(LoadSlotSwitcherViewModel);
	for (const auto& Pair : LoadSlotSwitcherViewModelMap)
	{
		// 关闭当前已选择存档槽位的选择按钮
		if (Pair.Value == LoadSlotSwitcherViewModel)
		{
			Pair.Value->BroadcastSelectButtonChange(false);
			continue;
		}
		// 开启其它槽位的存档选择按钮
		Pair.Value->BroadcastSelectButtonChange(true);
	}

	// 最后选择存档槽位且没有开始加载存档前,就设置好游戏实例里对应存档所保存角色的[默认出生位置],以及当前使用的存档槽位
	Gi->LoadSlotName   = LoadSlotSwitcherViewModel->GetLoadSlotName();
	Gi->LoadSlotIndex  = LoadSlotSwitcherViewModel->GetLoadSlotInSave();
	Gi->PlayerStartTag = LoadSlotSwitcherViewModel->GetPlayerStartTag();

	// 暂存当前选中的存档插槽ViewModel
	CurrentSelectedSlotSwitcher = LoadSlotSwitcherViewModel;
}

void UMVVMLoadScreen::DeleteButtonPressed()
{
	if (!CurrentSelectedSlotSwitcher.Get()) { return; }

	if (!GetMyGameMode()) { return; }

	const auto IsSuccess = GetMyGameMode()->DeleteSlotData(CurrentSelectedSlotSwitcher);

	// 成功删除则切换到空槽位
	if (IsSuccess)
	{
		CurrentSelectedSlotSwitcher->BroadcastSwitcherLoadSlotChange(
		                                                             FRPGAuraGameplayTags::Get().
		                                                             Widget_Menu_Load_Slot_Vacant);
	}

	// 不管删除失败还是成功,当前选择按钮都启用
	CurrentSelectedSlotSwitcher->BroadcastSelectButtonChange(true);
}

void UMVVMLoadScreen::StartButtonPressed()
{
	if (!IsValid(CurrentSelectedSlotSwitcher.Get())) { return; }

	if (!GetMyGameMode()) { return; }

	GetMyGameMode()->OpenLevel(CurrentSelectedSlotSwitcher->GetMapName());
}

void UMVVMLoadScreen::LoadSlotData()
{
	const auto GameMode = GetMyGameMode();
	const auto Gi       = GetMyGameInstance();
	if (!GameMode || !Gi) { return; }

	if (!LoadSlotSwitcherViewModelMap.Num())
	{
		UE_LOG(UMVVMLoadScreenLog, Error, TEXT("[%s]没有插槽切换器的数据!"), *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}

	for (const auto& Pair : LoadSlotSwitcherViewModelMap)
	{
		const auto LoadScreenSaveGameObj = GameMode->GetLoadSlotData(Pair.Value);

		if (!LoadScreenSaveGameObj) { continue; }
		// 从Viewmodel获取要设置保存的数据
		Pair.Value->SetPlayerLevel(LoadScreenSaveGameObj->PlayerLevel);
		Pair.Value->SetPlayerName(LoadScreenSaveGameObj->PlayerName);
		Pair.Value->SetMapName(LoadScreenSaveGameObj->MapName);
		Pair.Value->SetPlayerStartTag(LoadScreenSaveGameObj->PlayerStartTag);

		if (!LoadScreenSaveGameObj->CurrentLoadSlotSwitcherLoadSlotTag.
		                            MatchesTag(FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot))
		{
			UE_LOG(UMVVMLoadScreenLog, Warning, TEXT("[%s]无法加载槽位"), *FString::Printf(TEXT(__FUNCTION__)));
			continue;
		}
		Pair.Value->BroadcastSwitcherLoadSlotChange(FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot_Taken);
	}
}

ARPGAuraGameModeBase* UMVVMLoadScreen::GetMyGameMode()
{
	if (!MyGameMode) { MyGameMode = Cast<ARPGAuraGameModeBase>(UGameplayStatics::GetGameMode(this)); }
	return MyGameMode.Get();
}

UBaseGameInstance* UMVVMLoadScreen::GetMyGameInstance()
{
	if (!MyGameInstance) { MyGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this)); }
	return MyGameInstance.Get();
}
