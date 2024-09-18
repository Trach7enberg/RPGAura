// Copyright GGBAO 


#include "UI/Widgets/MainSpell.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "FunctionLibrary/WidgetControllerBpFuncLib.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "UI/WidgetControllers/SpellGlobeWidgetController.h"

DEFINE_LOG_CATEGORY_STATIC(UMainSpellLog, All, All);

void UMainSpell::InitOffensiveArea()
{
	if (!SpellGlobeWidgetClass || !GetOwningPlayer()) { return; }

	const auto Gi = GetOwningPlayer()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }

	auto TagToAbilityInfos = Gi->AbilityInfoAsset.Get()->AbilityInfos;

	UWidgetControllerBpFuncLib::CreateMultipleWidget<UHorizontalBoxSlot>(
		SpellGlobeWidgetClass, GetOwningPlayer(), OffensiveArea, OffensiveGlobeNum, TagToAbilityInfos,
		USpellGlobeWidgetController::StaticClass());
}

void UMainSpell::InitPassiveArea()
{
	if (!SpellGlobeWidgetClass || !GetOwningPlayer()) { return; }

	const auto Gi = GetOwningPlayer()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }
	
	auto TagToAbilityInfos = TArray<FTagToAbilityInfo>();

	UWidgetControllerBpFuncLib::CreateMultipleWidget<UVerticalBoxSlot>(
		SpellGlobeWidgetClass, GetOwningPlayer(), PassiveArea, PassiveGlobeNum, TagToAbilityInfos,
		USpellGlobeWidgetController::StaticClass());
}
