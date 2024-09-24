// Copyright GGBAO 


#include "UI/WidgetControllers/SpellButtonWidgetController.h"

#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(USpellButtonWidgetControllerLog, All, All);

void USpellButtonWidgetController::BindCallBack() {}

void USpellButtonWidgetController::BroadcastInitialValues() {}

void USpellButtonWidgetController::BroadCastSpellGlobeButtonSelectedChange(const FGameplayTag& WidgetTag,const FGameplayTag& AbilityStatusTag)
{
	if (!IsWidgetControllerParamsValid()) { return; }


	GetWidgetControllerParams().GameInstanceSubsystem->OnSpellButtonSelectedChange.Broadcast(this, WidgetTag,AbilityStatusTag);
}
