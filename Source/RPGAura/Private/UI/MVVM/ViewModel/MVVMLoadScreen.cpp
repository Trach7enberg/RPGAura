// Copyright GGBAO 


#include "UI/MVVM/ViewModel/MVVMLoadScreen.h"

#include "GameplayTagContainer.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "UI/MVVM/ViewModel/MVVMLoadSlotSwitcher.h"

DEFINE_LOG_CATEGORY_STATIC(UMVVMLoadScreenLog, All, All);

void UMVVMLoadScreen::InitialLoadSlotSwitcherWidgetViewModel()
{
	if(!UmvvmLoadSlotSwitcherClass)
	{
		UE_LOG(UMVVMLoadScreenLog, Error, TEXT("[%s]ViewModel Class 为Nullptr!"), *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}

	// 每一个插槽切换器对应的ViewModel
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_1, NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass));
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_2, NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass));
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_3, NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass));
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

void UMVVMLoadScreen::NewSaveButtonPressed(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel, const FString& NewSaveName)
{
	
}

void UMVVMLoadScreen::NewGameButtonPressed(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel)
{
	
	if(!LoadSlotSwitcherViewModel){return;}

	LoadSlotSwitcherViewModel->OnSetWidgetSwitcher.Broadcast(FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot_Enter);
}
void UMVVMLoadScreen::SelectLoadButtonPressed(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel, const FString& NewSaveName) {}
