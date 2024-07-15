// Copyright GGBAO 


#include "UI/WidgetControllers/MainWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
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

	OnHealthChangedSignature.Broadcast(MyAs->GetCurrentHealth(), false);
	OnMaxHealthChangedSignature.Broadcast(MyAs->GetMaxHealth(), false);

}

void UMainWidgetController::BindCallBackToGas()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UMainWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if (!MyAsc) { return; }

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs) { return; }

	// 每当CurrentHealth属性的值改变,就会调用回调函数
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetCurrentHealthAttribute()).AddUObject(
		                            this, &UMainWidgetController::HealthChanged);
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxHealthAttribute()).AddUObject(
		                            this, &UMainWidgetController::MaxHealthChanged);
}

void UMainWidgetController::HealthChanged(const FOnAttributeChangeData &Data) const
{
	OnHealthChangedSignature.Broadcast(Data.NewValue,
	                                   Data.NewValue > Data.OldValue);
}

void UMainWidgetController::MaxHealthChanged(const FOnAttributeChangeData &Data) const
{
	OnMaxHealthChangedSignature.Broadcast(Data.NewValue,
	                                      Data.NewValue > Data.OldValue);
}
