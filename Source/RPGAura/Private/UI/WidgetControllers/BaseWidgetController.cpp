// Copyright GGBAO 


#include "UI/WidgetControllers/BaseWidgetController.h"

void UBaseWidgetController::BroadcastInitialValues() {}

bool UBaseWidgetController::IsWidgetControllerParamsValid() const
{
	return GetWidgetControllerParams().CurrentPlayerController && GetWidgetControllerParams().CurrentPlayerState &&
		GetWidgetControllerParams().CurrentAbilitySystemComponent && GetWidgetControllerParams().CurrentAttributeSet;
}

void UBaseWidgetController::BindCallBack() {}
