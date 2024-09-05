// Copyright GGBAO 


#include "UI/WidgetControllers/MainSpellWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(UMainSpellWidgetControllerLog, All, All);

void UMainSpellWidgetController::BindCallBack()
{
	
}
void UMainSpellWidgetController::BroadcastInitialValues()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UMainSpellWidgetControllerLog, Error, TEXT("[%s] , [%hs]Widget控制器参数无效"), *GetNameSafe(this),
			   __FUNCTION__);
		return;
	}

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if(!MyAsc){return;}

	MyAsc->BroadCastDefaultActivatableAbilitiesInfo();
}
