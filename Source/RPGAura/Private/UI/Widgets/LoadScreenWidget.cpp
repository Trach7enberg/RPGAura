// Copyright GGBAO 


#include "UI/Widgets/LoadScreenWidget.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "UI/MVVM/ViewModel/MVVMLoadSlotSwitcher.h"
#include "UI/Widgets/LoadSlotSwitcherWidget.h"

DEFINE_LOG_CATEGORY_STATIC(ULoadScreenWidgetLog, All, All);

void ULoadScreenWidget::InitializeLoadSlotSwitcherWidget()
{
	if (!UmvvmLoadSlotSwitcherClass)
	{
		UE_LOG(ULoadScreenWidgetLog, Error, TEXT("[%s]ViewModel生成类为null!"), *FString::Printf(TEXT(__FUNCTION__)));
	}

	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_1,
	                                 NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass));
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_2,
	                                 NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass));
	LoadSlotSwitcherViewModelMap.Add(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_3,
	                                 NewObject<UMVVMLoadSlotSwitcher>(this, UmvvmLoadSlotSwitcherClass));

	// 设置切换器的Widget Tag
	LoadSlotSwitcher1.Get()->SetCurrentGameplayTag(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_1);
	LoadSlotSwitcher2.Get()->SetCurrentGameplayTag(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_2);
	LoadSlotSwitcher3.Get()->SetCurrentGameplayTag(FRPGAuraGameplayTags::Get().Widget_Menu_Load_SlotSwitcher_3);

}

UMVVMLoadSlotSwitcher* ULoadScreenWidget::GetSwitcherViewModelByTag(FGameplayTag WidgetTag)
{
	auto ViewModel = LoadSlotSwitcherViewModelMap.Find(WidgetTag);
	if (!ViewModel)
	{
		UE_LOG(ULoadScreenWidgetLog, Error, TEXT("[%s]无法获取ViewModel!"), *FString::Printf(TEXT(__FUNCTION__)));

		return nullptr;
	}
	return *ViewModel;
}
