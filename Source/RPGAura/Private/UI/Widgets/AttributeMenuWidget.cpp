// Copyright GGBAO 


#include "UI/Widgets/AttributeMenuWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/VerticalBox.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "UI/WidgetControllers/AttributeMenuWidgetController.h"

DEFINE_LOG_CATEGORY_STATIC(UAttributeMenuWidgetLog, All, All);

void UAttributeMenuWidget::InitVerticalArea()
{
    if (!TextValueRowButtonWidgetClass || !TextValueRowWidgetClass || !GetOwningPlayer())
    {
        UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("致命错误!"))
        return;
    }

    const auto CurrentAttributeMenuWidgetController = Cast<UAttributeMenuWidgetController>(GetWidgetController());
    if (!CurrentAttributeMenuWidgetController || !CurrentAttributeMenuWidgetController->IsWidgetControllerParamsValid())
    {
        UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("需要先设置widget控制器和widget控制器参数!"))
        return;
    }

    CurrentAttributePointArea->ClearChildren();

    // 创建可加属性点数区域
    CurrentAttributePointRow = CreateWidget<UBaseUserWidget>(GetOwningPlayer(), TextValueRowWidgetClass);
    CurrentAttributePointArea->AddChild(CurrentAttributePointRow);

    // 创建主要属性widget
    CreateVerticalAreaWidgets(FRPGAuraGameplayTags::
                           GetGameplayTagsByType(EGameplayTagType::PrimaryGameplayTags),
                           EGameplayTagType::PrimaryGameplayTags,
                           VerticalBox_PrimaryAttriArea);

    // 创建次要属性widget
    VerticalBox_SecondaryAttriArea->ClearChildren();
    CreateVerticalAreaWidgets(FRPGAuraGameplayTags::
                           GetGameplayTagsByType(EGameplayTagType::SecondaryGameplayTags),
                           EGameplayTagType::SecondaryGameplayTags,
                           VerticalBox_SecondaryAttriArea);
}

UBaseUserWidget* UAttributeMenuWidget::GetWidgetByGameplayTag(const EGameplayTagType GameplayTagType,
                                                              const FGameplayTag Tag)
{
    UBaseUserWidget* Result = nullptr;
    switch (GameplayTagType)
    {
        case EGameplayTagType::AllGameplayTags:
            Result = FindWidgetByTagFromArray(AllAttriAreaWidgetArray, Tag);
            break;
        case EGameplayTagType::VitalGameplayTags:
            Result = FindWidgetByTagFromArray(VitalAttriAreaWidgetArray, Tag);
            break;
        case EGameplayTagType::PrimaryGameplayTags:
            Result = FindWidgetByTagFromArray(PrimaryAttriAreaWidgetArray, Tag);
            break;
        case EGameplayTagType::SecondaryGameplayTags:
            Result = FindWidgetByTagFromArray(SecondaryAttriAreaWidgetArray, Tag);
            break;
        default: ;
    }
    return Result;
}

UBaseUserWidget* UAttributeMenuWidget::FindWidgetByTagFromArray(const TArray<UBaseUserWidget*>& Array,
                                                                const FGameplayTag& Tag)
{
    for (const auto Widget : Array)
    {
        if (Widget->GetCurrentGameplayTag() == Tag)
        {
            return Widget;
        }
    }
    return nullptr;
}

void UAttributeMenuWidget::CreateVerticalAreaWidgets(const TArray<FGameplayTag*>* Array,
                                                  const EGameplayTagType GameplayTagType,
                                                  UVerticalBox* VerticalArea)
{
    if (!Array || !VerticalArea)
    {
        UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("标签数组和垂直区域内容不能为空!"))
        return;
    }
    VerticalArea->ClearChildren();
    for (const auto GameplayTag : *Array)
    {
        const auto Widget = CreateWidget<UBaseUserWidget>(GetOwningPlayer(), TextValueRowButtonWidgetClass);
        Widget->SetCurrentGameplayTag(*GameplayTag);
        VerticalArea->AddChild(Widget);

        switch (GameplayTagType)
        {
            case EGameplayTagType::AllGameplayTags:
                break;
            case EGameplayTagType::VitalGameplayTags:
                VitalAttriAreaWidgetArray.Add(Widget);
                break;
            case EGameplayTagType::PrimaryGameplayTags:
                PrimaryAttriAreaWidgetArray.Add(Widget);
                Widget->SetPadding(FMargin(0, 0, 14, 0));
                break;
            case EGameplayTagType::SecondaryGameplayTags:
                SecondaryAttriAreaWidgetArray.Add(Widget);
                break;
        }

        AllAttriAreaWidgetArray.Add(Widget);
    }
}