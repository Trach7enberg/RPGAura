// Copyright GGBAO 


#include "UI/WidgetControllers/EquippedAbilityWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(UEquippedAbilityWidgetControllerLog, All, All);

void UEquippedAbilityWidgetController::BindCallBack()
{
	
}
void UEquippedAbilityWidgetController::BroadcastInitialValues()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UEquippedAbilityWidgetControllerLog, Error, TEXT("[%s] , [%hs]Widget控制器参数无效"), *GetNameSafe(this),
			   __FUNCTION__);
		return;
	}

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if(!MyAsc){return;}

	MyAsc->BroadCastDefaultActivatableAbilitiesInfo();
}
