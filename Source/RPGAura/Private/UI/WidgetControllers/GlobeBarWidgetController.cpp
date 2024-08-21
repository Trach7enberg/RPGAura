// Copyright GGBAO 


#include "UI/WidgetControllers/GlobeBarWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"

DEFINE_LOG_CATEGORY_STATIC(UGlobeBarWidgetControllerLog, All, All);

void UGlobeBarWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UGlobeBarWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto Asc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if (!Asc) { return; }

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs) { return; }

	// 每当CurrentHealth属性的值改变,就会调用回调函数
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetCurrentHealthAttribute()).AddUObject(
		                            this,
		                            &UGlobeBarWidgetController::HealthChanged);
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxHealthAttribute()).AddUObject(
		                            this,
		                            &UGlobeBarWidgetController::MaxHealthChanged);

	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetCurrentManaAttribute()).AddUObject(
		                            this,
		                            &UGlobeBarWidgetController::ManaChanged);
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxManaAttribute()).AddUObject(
		                            this,
		                            &UGlobeBarWidgetController::MaxManaChanged);
}

void UGlobeBarWidgetController::BroadcastInitialValues()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UGlobeBarWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs) { return; }

	OnHealthChangedSignature.Broadcast(MyAs->GetCurrentHealth(), false);
	OnMaxHealthChangedSignature.Broadcast(MyAs->GetMaxHealth(), false);
	OnManaChangedSignature.Broadcast(MyAs->GetCurrentMana(), false);
	OnMaxManaChangedSignature.Broadcast(MyAs->GetMaxMana(), false);
}

void UGlobeBarWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedSignature.Broadcast(Data.NewValue,
	                                   Data.NewValue > Data.OldValue);
}

void UGlobeBarWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedSignature.Broadcast(Data.NewValue,
	                                      Data.NewValue > Data.OldValue);
}

void UGlobeBarWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChangedSignature.Broadcast(Data.NewValue,
	                                 Data.NewValue > Data.OldValue);
}

void UGlobeBarWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChangedSignature.Broadcast(Data.NewValue,
	                                    Data.NewValue > Data.OldValue);
}
