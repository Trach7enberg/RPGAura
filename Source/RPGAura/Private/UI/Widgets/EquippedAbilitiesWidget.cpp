// Copyright GGBAO 


#include "UI/Widgets/EquippedAbilitiesWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "FunctionLibrary/WidgetControllerBpFuncLib.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "UI/WidgetControllers/EquippedAbilityWidgetController.h"

class URPGAuraGameInstanceSubsystem;
DEFINE_LOG_CATEGORY_STATIC(UEquippedAbilitiesWidgetLog, All, All);

void UEquippedAbilitiesWidget::InitOffensiveEquippedArea()
{
	if (!EquippedSpell || !OffensiveEquippedArea || !GetOwningPlayer())
	{
		UE_LOG(UEquippedAbilitiesWidgetLog, Error, TEXT("[%s]致命错误"), *GetNameSafe(this));
		return;
	}

	const auto Gi = GetOwningPlayer()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }

	auto TagToAbilityInfos = Gi->AbilityInfoAsset.Get()->AbilityInfos;

	UWidgetControllerBpFuncLib::CreateMultipleWidget<UHorizontalBoxSlot>(
		EquippedSpell, GetOwningPlayer(), OffensiveEquippedArea.Get()
		, EquippedAbilityNum_Offensive, TagToAbilityInfos, UEquippedAbilityWidgetController::StaticClass(),
		HAlign_Center, VAlign_Center);
}

void UEquippedAbilitiesWidget::InitPassiveEquippedArea()
{
	if (!EquippedSpell || !PassiveEquippedArea || !GetOwningPlayer())
	{
		UE_LOG(UEquippedAbilitiesWidgetLog, Error, TEXT("[%s]致命错误"), *GetNameSafe(this));
		return;
	}
	const auto Gi = GetOwningPlayer()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }

	auto TagToAbilityInfos = TArray<FTagToAbilityInfo>();
	UWidgetControllerBpFuncLib::CreateMultipleWidget<UVerticalBoxSlot>(
		EquippedSpell, GetOwningPlayer(), PassiveEquippedArea
		, EquippedAbilityNum_Passive, TagToAbilityInfos, UEquippedAbilityWidgetController::StaticClass());
}
