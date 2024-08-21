// Copyright GGBAO 


#include "UI/WidgetControllers/MainWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/PickupMessageAsset.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"


DEFINE_LOG_CATEGORY_STATIC(UMainWidgetControllerLog, All, All);

void UMainWidgetController::BroadcastInitialValues()
{
    
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


    // 绑定委托
    MyAsc->OnGetAssetTagsDelegate.AddUObject(this, &UMainWidgetController::OnGetAssetTags);
}

void UMainWidgetController::OnGetAssetTags(const FGameplayTagContainer& AssetTags)
{
    if(!IsWidgetControllerParamsValid())
    {
        UE_LOG(UMainWidgetControllerLog, Error, TEXT("控制器的基本参数无效!"));
        return;
    }
    auto GiSubSystem = GetWidgetControllerParams().CurrentPlayerController->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
    if(!GiSubSystem || !GiSubSystem->MessageWidgetDataAsset)
    {
        UE_LOG(UMainWidgetControllerLog, Error, TEXT("子系统或者数据资产获取失败!"));
        return;
    }
    
    for (auto AssetTag : AssetTags)
    {
        const auto MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));

        // 如果当前获得的资产Tag不是Message开头的标签的话就跳过
        if (AssetTag.MatchesTag(MessageTag))
        {
            const auto Row = GiSubSystem->MessageWidgetDataAsset.Get()->FindPickupMessageByTag(AssetTag);
            // 广播当前数据表中的表行
            OnMessageWidgetRow.Broadcast(Row);
        }
    }
}
