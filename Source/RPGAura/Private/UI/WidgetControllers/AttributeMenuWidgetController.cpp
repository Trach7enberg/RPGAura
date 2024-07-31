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

    const auto MyAcs = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
    if (!MyAcs)
    {
        return;
    }

    // UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("ALL:%d"), FRPGAuraGameplayTags::GetGameplayTagsMap().Num());
    UE_LOG(UAttributeMenuWidgetControllerLog, Error, TEXT("ALL:%d"), MyAs->TagToAttributeMap.Num());

    // 遍历属性集的主要和次要属性,广播属性信息
    // DoFirstBroadcast(MyAs, FRPGAuraGameplayTags::PrimaryGameplayTagsArray);
    // DoFirstBroadcast(MyAs, FRPGAuraGameplayTags::SecondaryGameplayTagsArray);
    for (const auto& Pair : MyAs->TagToAttributeMap)
    {
        BroadcastAttributeInfoStruct(MyAs, Pair);
    }
}

void UAttributeMenuWidgetController::BindCallBack()
{
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

    for (const auto& Pair : MyAs->TagToAttributeMap)
    {
        HandleAnyAttributeChange(MyAs, Pair);
    }
}

void UAttributeMenuWidgetController::DoFirstBroadcast(const UBaseAttributeSet* MyAs, const TArray<FGameplayTag*>& TagArray) const
{
    for (const auto Tag : TagArray)
    {
        const auto AttributePtr = MyAs->TagToAttributeMap.Find(*Tag);
        if (!AttributePtr)
        {
            UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("找不到GT:[%s]对应的属性"), *Tag->GetTagName().ToString());
            continue;
        }

        const auto AttributeInfoStruct = AttributeInfo->FindAttributeInfoByTag(*Tag);
        if (!AttributeInfoStruct)
        {
            UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("找不到GT:[%s]对应的属性信息结构体"), *Tag->GetTagName().ToString());
            return;
        }

        AttributeInfoStruct->AttributeValue = (*AttributePtr)().GetNumericValue(MyAs);
        OnAttributeChanged.Broadcast(*AttributeInfoStruct);
    }
}

void UAttributeMenuWidgetController::HandleAnyAttributeChange(const FOnAttributeChangeData& Data) const
{
    const auto AttributeInfoStruct = AttributeInfo->FindAttributeInfoByAttributeName(
        Data.Attribute.GetName());

    if (!AttributeInfoStruct)
    {
        UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("映射属性信息结构无效!"));
        return;
    }

    AttributeInfoStruct->AttributeValue = Data.NewValue;
    OnAttributeChanged.Broadcast(*AttributeInfoStruct);
}

void UAttributeMenuWidgetController::HandleAnyAttributeChange(const UBaseAttributeSet* MyAs, const TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair) const
{
    GetWidgetControllerParams().CurrentAbilitySystemComponent->
                                GetGameplayAttributeValueChangeDelegate(Pair.Value())
                                .AddLambda([this,Pair,MyAs](const FOnAttributeChangeData& Data)
                                {
                                    BroadcastAttributeInfoStruct(MyAs, Pair);
                                });
}

void UAttributeMenuWidgetController::BroadcastAttributeInfoStruct(const UBaseAttributeSet* MyAs, const TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair) const
{
    const auto AttributeInfoStruct = AttributeInfo->FindAttributeInfoByTag(Pair.Key);
    if (!AttributeInfoStruct)
    {
        UE_LOG(UAttributeMenuWidgetControllerLog, Warning, TEXT("找不到GT:[%s]对应的属性信息结构体"), *Pair.Key.ToString());
        return;
    }

    AttributeInfoStruct->AttributeValue = Pair.Value().GetNumericValue(MyAs);
    OnAttributeChanged.Broadcast(*AttributeInfoStruct);
}