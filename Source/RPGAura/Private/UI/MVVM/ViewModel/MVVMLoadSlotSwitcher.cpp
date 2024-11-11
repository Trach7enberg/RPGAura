// Copyright GGBAO 


#include "UI/MVVM/ViewModel/MVVMLoadSlotSwitcher.h"

#include "CoreTypes/RPGAuraGameplayTags.h"

DEFINE_LOG_CATEGORY_STATIC(UMVVMLoadSlotSwitcherLog, All, All);

UMVVMLoadSlotSwitcher::UMVVMLoadSlotSwitcher()
{
	CurrentLoadSlotSwitcherLoadSlot = FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot_Vacant;
}

void UMVVMLoadSlotSwitcher::BroadcastSwitcherLoadSlotChange(const FGameplayTag& SlotTag) 
{
	if (!GetCurrentLoadSlotSwitcherLoadSlotTag().IsValid() || !SlotTag.MatchesTag(FRPGAuraGameplayTags::Get().Widget_Menu_Load_Slot)) { return; }
	SetCurrentLoadSlotSwitcherLoadSlotTag(SlotTag);
	OnSlotSwitcherSelected.Broadcast(GetCurrentLoadSlotSwitcherLoadSlotTag());
}

void UMVVMLoadSlotSwitcher::BroadcastSelectButtonChange(const bool& Enabled)
{
	OnSelectButtonChange.Broadcast(Enabled);
}
