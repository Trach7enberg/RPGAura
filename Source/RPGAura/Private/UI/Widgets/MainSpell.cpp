// Copyright GGBAO 


#include "UI/Widgets/MainSpell.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/Spacer.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
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

	const auto TagToAbilityInfos = Gi->AbilityInfoAsset.Get()->AbilityInfos;

	OffensiveArea->ClearChildren();

	for (int i = 0; i < OffensiveGlobeNum; ++i)
	{
		const auto Widget = CreateWidget<UBaseUserWidget>(this, SpellGlobeWidgetClass);
		if (!Widget) { break; }
		if (i < TagToAbilityInfos.Num()) { Widget->SetCurrentGameplayTag(TagToAbilityInfos[i].InputTag); }
		Widget->SetWidgetController(
			UWidgetControllerBpFuncLib::CreateWidgetController(USpellGlobeWidgetController::StaticClass(),
			                                                   GetOwningPlayer()));
		OffensiveArea->AddChild(Widget);
	}
}

void UMainSpell::InitPassiveArea()
{
	if (!SpellGlobeWidgetClass || !GetOwningPlayer()) { return; }

	const auto Gi = GetOwningPlayer()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }

	const auto TagToAbilityInfos = Gi->AbilityInfoAsset.Get()->AbilityInfos;

	PassiveArea->ClearChildren();

	for (int i = 0; i < PassiveGlobeNum; ++i)
	{
		const auto Widget = CreateWidget<UBaseUserWidget>(this, SpellGlobeWidgetClass);
		if (!Widget) { break; }
		
		Widget->SetWidgetController(
			UWidgetControllerBpFuncLib::CreateWidgetController(USpellGlobeWidgetController::StaticClass(),
			                                                   GetOwningPlayer()));
		PassiveArea->AddChild(Widget);
		const auto t = Cast<UVerticalBoxSlot>(Widget->Slot.Get());
		if (t)
		{
			t->SetSize(FSlateChildSize{ESlateSizeRule::Fill});
			t->SetHorizontalAlignment(HAlign_Center);
			t->SetVerticalAlignment(VAlign_Center);
		}
	}

	// 创建UI中的Spacer 
	const auto Spacer = WidgetTree.Get()->ConstructWidget<USpacer>();
	if(!Spacer){return;}
	PassiveArea->AddChild(Spacer);
	
	const auto SpacerParentSlot = Cast<UVerticalBoxSlot>(Spacer->Slot.Get());
	if (SpacerParentSlot)
	{
		FSlateChildSize SizeRule = FSlateChildSize(ESlateSizeRule::Fill);
		SizeRule.Value = 0.7f;
		SpacerParentSlot->SetSize(SizeRule);
	}
}
