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
	if (!MyAs)
	{
		return;
	}

	OnHealthChangedSignature.Broadcast(MyAs->GetCurrentHealth(), false);
	OnMaxHealthChangedSignature.Broadcast(MyAs->GetMaxHealth(), false);
	OnManaChangedSignature.Broadcast(MyAs->GetCurrentMana(), false);
	OnMaxManaChangedSignature.Broadcast(MyAs->GetMaxMana(), false);

}

void UMainWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UMainWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto Asc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if (!Asc)
	{
		return;
	}

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(Asc);
	if (!MyAsc)
	{
		return;
	}

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs)
	{
		return;
	}

	// 每当CurrentHealth属性的值改变,就会调用回调函数
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetCurrentHealthAttribute()).AddUObject(
		                            this, &UMainWidgetController::HealthChanged);
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxHealthAttribute()).AddUObject(
		                            this, &UMainWidgetController::MaxHealthChanged);

	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetCurrentManaAttribute()).AddUObject(
		                            this, &UMainWidgetController::ManaChanged);
	GetWidgetControllerParams().CurrentAbilitySystemComponent->
	                            GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxManaAttribute()).AddUObject(
		                            this, &UMainWidgetController::MaxManaChanged);


	// 绑定委托
	MyAsc->OnGetAssetTagsDelegate.AddUObject(this, &UMainWidgetController::OnGetAssetTags);
	

}

void UMainWidgetController::OnGetAssetTags(const FGameplayTagContainer &AssetTags)
{
	for (auto AssetTag : AssetTags)
	{

		const auto MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

		// 如果当前获得的资产Tag不是Message开头的标签的话就跳过
		if (AssetTag.MatchesTag(MessageTag))
		{
			const auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, AssetTag);
			if (Row)
			{
				// 广播当前数据表中的表行
				OnMessageWidgetRow.Broadcast(*Row);
			}
		}
	}

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

void UMainWidgetController::ManaChanged(const FOnAttributeChangeData &Data) const
{
	OnManaChangedSignature.Broadcast(Data.NewValue,
	                                 Data.NewValue > Data.OldValue);
}

void UMainWidgetController::MaxManaChanged(const FOnAttributeChangeData &Data) const
{
	OnMaxManaChangedSignature.Broadcast(Data.NewValue,
	                                    Data.NewValue > Data.OldValue);
}
