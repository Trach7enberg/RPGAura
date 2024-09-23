// Copyright GGBAO 


#include "UI/Widgets/MainSpell.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "FunctionLibrary/WidgetControllerBpFuncLib.h"
#include "UI/WidgetControllers/SpellGlobeWidgetController.h"

DEFINE_LOG_CATEGORY_STATIC(UMainSpellLog, All, All);

void UMainSpell::InitOffensiveArea()
{
	if (!SpellGlobeWidgetClass || !GetOwningPlayer()) { return; }

	UWidgetControllerBpFuncLib::CreateMultipleWidget<UHorizontalBoxSlot>(
		SpellGlobeWidgetClass, GetOwningPlayer(), OffensiveArea, OffensiveGlobeNum, FRPGAuraGameplayTags::Get().InputOffensiveTagsContainer,
		USpellGlobeWidgetController::StaticClass());
}

void UMainSpell::InitPassiveArea()
{
	if (!SpellGlobeWidgetClass || !GetOwningPlayer()) { return; }

	UWidgetControllerBpFuncLib::CreateMultipleWidget<UVerticalBoxSlot>(
		SpellGlobeWidgetClass, GetOwningPlayer(), PassiveArea, PassiveGlobeNum, FRPGAuraGameplayTags::Get().PassiveTagsContainer,
		USpellGlobeWidgetController::StaticClass());
}
