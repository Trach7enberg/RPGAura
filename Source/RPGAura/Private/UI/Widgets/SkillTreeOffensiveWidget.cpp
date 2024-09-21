// Copyright GGBAO 


#include "UI/Widgets/SkillTreeOffensiveWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBox.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "FunctionLibrary/WidgetControllerBpFuncLib.h"
#include "UI/WidgetControllers/SpellButtonWidgetController.h"

DEFINE_LOG_CATEGORY_STATIC(USkillTreeOffensiveWidgetLog, All, All);

void USkillTreeOffensiveWidget::InitSkillTreeArea()
{
	CreateTreeAreaWidget(FireSpellArea, FRPGAuraGameplayTags::Get().AttackSpellFireTagsContainer);
	CreateTreeAreaWidget(LightingArea, FRPGAuraGameplayTags::Get().AttackSpellLightningTagsContainer);
	CreateTreeAreaWidget(ArcaneArea, FRPGAuraGameplayTags::Get().AttackSpellArcaneTagsContainer);
}

void USkillTreeOffensiveWidget::CreateTreeAreaWidget(UPanelWidget* Area, const FGameplayTagContainer& WidgetTags) const
{
	if (!GetOwningPlayer() || !WidgetTree || !Area || !SpellButtonClass || !SkillDirectionTexture) { return; }
	Area->ClearChildren();

	for (int i = 1; i <= RowNum; ++i)
	{
		const auto SpellButton = CreateWidget<UBaseUserWidget>(GetOwningPlayer(), SpellButtonClass);
		if (!SpellButton) { continue; }

		if (WidgetTags.IsValidIndex(i - 1)) { SpellButton->SetCurrentGameplayTag(WidgetTags.GetByIndex(i - 1)); }
		SpellButton->SetWidgetController(
			UWidgetControllerBpFuncLib::CreateWidgetController(USpellButtonWidgetController::StaticClass(),
			                                                   GetOwningPlayer()));

		Area->AddChild(SpellButton);

		// 添加技能树的路径线图片
		if (i != RowNum)
		{
			const auto SkillDirectionImg = WidgetTree.Get()->ConstructWidget<UImage>(UImage::StaticClass());
			if (!SkillDirectionImg) { continue; }	
			SkillDirectionImg->SetBrushFromTexture(SkillDirectionTexture);
			SkillDirectionImg->SetDesiredSizeOverride(FVector2D(32.f, 35.f));
			Area->AddChild(SkillDirectionImg);
		}
	}
}
