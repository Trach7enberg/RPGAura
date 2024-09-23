// Copyright GGBAO 


#include "UI/Widgets/EquippedAbilitiesWidget.h"

#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
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

	UWidgetControllerBpFuncLib::CreateMultipleWidget<UHorizontalBoxSlot>(
		EquippedSpell, GetOwningPlayer(), OffensiveEquippedArea.Get()
		, EquippedAbilityNum_Offensive, FRPGAuraGameplayTags::Get().InputOffensiveTagsContainer, UEquippedAbilityWidgetController::StaticClass(),
		HAlign_Center, VAlign_Center);
}

void UEquippedAbilitiesWidget::InitPassiveEquippedArea()
{
	if (!EquippedSpell || !PassiveEquippedArea || !GetOwningPlayer())
	{
		UE_LOG(UEquippedAbilitiesWidgetLog, Error, TEXT("[%s]致命错误"), *GetNameSafe(this));
		return;
	}
	
	UWidgetControllerBpFuncLib::CreateMultipleWidget<UVerticalBoxSlot>(
		EquippedSpell, GetOwningPlayer(), PassiveEquippedArea
		, EquippedAbilityNum_Passive, FRPGAuraGameplayTags::Get().PassiveTagsContainer, UEquippedAbilityWidgetController::StaticClass());
}
