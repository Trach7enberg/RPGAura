// Copyright GGBAO 


#include "UI/WidgetControllers/AttributeMenuWidgetController.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/AttributeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(UAttributeMenuWidgetControllerLog, All, All);

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	if (!AttributeInfo)
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("映射数据资产表信息不能为nullptr!"));
		return;
	}

	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
		return;
	}

	const auto MyAs = Cast<UBaseAttributeSet>(GetWidgetControllerParams().CurrentAttributeSet);
	if (!MyAs)
	{
		return;
	}

	TArray<FGameplayTag *> TagsArray;
	FRPGAuraGameplayTags::Get().GetAllGameplayTags(TagsArray);
	UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("ALL:%d"), TagsArray.Num());


	TArray<FGameplayAttribute> Array;
	GetWidgetControllerParams().CurrentAbilitySystemComponent->GetAllAttributes(Array);
	for (auto &Attribute : Array)
	{
		const auto AttributeInfoStruct = AttributeInfo->GetAttributeInfoByAttributeName(Attribute.GetName());

		if (!AttributeInfoStruct)
		{
			UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("找不到[%s]对应的属性信息结构体"),*Attribute.GetName());
			continue;
		}
		
		AttributeInfoStruct->AttributeValue = Attribute.GetNumericValue(MyAs);
		OnAttributeChanged.Broadcast(*AttributeInfoStruct);
		// UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("%s"), *AttributeInfoStruct->AttributeName.ToString());

	}


}

void UAttributeMenuWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
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

	{
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
								   GetGameplayAttributeValueChangeDelegate(MyAs->GetStrengthAttribute())
								   .AddLambda([this](const FOnAttributeChangeData &Data)
								   {
									   HandleAnyAttributeChange(Data);
								   });
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetIntelligenceAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetResilienceAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetVigorAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
	}

	{
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetArmorAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetArmorPenetrationAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetBlockChanceAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetCriticalHitChanceAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetCriticalHitResistanceAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetCriticalHitDamageAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetHealthRegenerationAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetManaRegenerationAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxHealthAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
		GetWidgetControllerParams().CurrentAbilitySystemComponent->
									GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxManaAttribute())
									.AddLambda([this](const FOnAttributeChangeData &Data)
									{
										HandleAnyAttributeChange(Data);
									});
	}
}

void UAttributeMenuWidgetController::HandleAnyAttributeChange(const FOnAttributeChangeData &Data) const
{
	const auto AttributeInfoStruct = AttributeInfo->GetAttributeInfoByAttributeName(
		Data.Attribute.GetName());
	if (!AttributeInfoStruct)
	{
		UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("映射属性信息结构无效!"));
		return;
	}
	AttributeInfoStruct->AttributeValue = Data.NewValue;
	OnAttributeChanged.Broadcast(*AttributeInfoStruct);
}
