// Copyright GGBAO 


#include "UI/Widgets/SkillTreePassiveWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Spacer.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "FunctionLibrary/WidgetControllerBpFuncLib.h"
#include "UI/WidgetControllers/SpellButtonWidgetController.h"

void USkillTreePassiveWidget::InitSkillTreeArea()
{
	CreateTreeAreaWidget(SpellArea, FRPGAuraGameplayTags::Get().PassiveTagsContainer);
}

void USkillTreePassiveWidget::SetSpacerSlotSize(const USpacer* Spacer, const FSlateChildSize Size) const
{
	if (!Spacer) { return; }
	const auto HorizSlot = Cast<UHorizontalBoxSlot>(Spacer->Slot.Get());
	if (!HorizSlot) { return; }
	
	HorizSlot->SetSize(Size);
}

void USkillTreePassiveWidget::CreateTreeAreaWidget(UPanelWidget* Area, const FGameplayTagContainer& WidgetTags) const
{
	if (!GetOwningPlayer() || !WidgetTree || !Area || !SpellButtonClass) { return; }
	Area->ClearChildren();

	// 创建Spacer
	auto Spacer = WidgetTree.Get()->ConstructWidget<USpacer>(USpacer::StaticClass());
	const auto SlotSize = FSlateChildSize(ESlateSizeRule::Fill);
	
	if (!Spacer) { return; }
	Area->AddChild(Spacer);
	
	SetSpacerSlotSize(Spacer, SlotSize);

	for (int i = 1; i <= ColNum; ++i)
	{
		const auto SpellButton = CreateWidget<UBaseUserWidget>(GetOwningPlayer(), SpellButtonClass);
		if (!SpellButton) { continue; }

		if (WidgetTags.IsValidIndex(i - 1)) { SpellButton->SetCurrentGameplayTag(WidgetTags.GetByIndex(i - 1)); }

		SpellButton->SetWidgetController(
			UWidgetControllerBpFuncLib::CreateWidgetController(USpellButtonWidgetController::StaticClass(),
			                                                   GetOwningPlayer()));
		Area->AddChild(SpellButton);

		Spacer = WidgetTree.Get()->ConstructWidget<USpacer>(USpacer::StaticClass());
		if (!Spacer) { continue; }
		Area->AddChild(Spacer);
		SetSpacerSlotSize(Spacer, SlotSize);
	}
}
