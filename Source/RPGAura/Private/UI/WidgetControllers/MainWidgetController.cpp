// Copyright GGBAO 


#include "UI/WidgetControllers/MainWidgetController.h"

#include "GAS/AttributeSet/BaseAttributeSet.h"


DEFINE_LOG_CATEGORY_STATIC(UMainWidgetControllerLog, All, All);

void UMainWidgetController::BroadcastInitialValues()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UMainWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs) { return; }

	OnHealthChangedSignature.Broadcast(MyAs->GetCurrentHealth(),
	                                   MyAs->CurrentHealth.GetCurrentValue() > MyAs->CurrentHealth.GetBaseValue());
	OnMaxHealthChangedSignature.Broadcast(MyAs->GetMaxHealth(),
	                                   MyAs->MaxHealth.GetCurrentValue() > MyAs->MaxHealth.GetBaseValue());
}
