// Copyright GGBAO 


#include "UI/WidgetControllers/EsmWidgetController.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

void UEsmWidgetController::BindCallBack() {}

void UEsmWidgetController::BroadcastInitialValues()
{
	if (!IsWidgetControllerParamsValid()) { return; }
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if (!MyAsc) { return; }

	// 广播技能树区域(包括被动和主动)的技能按钮更新显示所需要的能力信息
	MyAsc->BroadCastDefaultSpellButtonAbilitiesInfo();
}


