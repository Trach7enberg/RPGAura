// Copyright GGBAO 


#include "UI/Widgets/AttributeMenuWidget.h"

#include "AbilitySystemComponent.h"
#include "Components/VerticalBox.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "FunctionLibrary/WidgetControllerBpFuncLib.h"
#include "UI/WidgetControllers/TextValueWidgetController.h"

DEFINE_LOG_CATEGORY_STATIC(UAttributeMenuWidgetLog, All, All);

void UAttributeMenuWidget::InitVerticalArea()
{
	if (!TextValueRowButtonWidgetClass || !TextValueRowWidgetClass || !GetOwningPlayer())
	{
		UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("致命错误!"))
		return;
	}

	CurrentAttributePointArea->ClearChildren();

	// 创建可加属性点数区域
	CurrentAttributePointRow = CreateWidget<UBaseUserWidget>(GetOwningPlayer(), TextValueRowWidgetClass);
	CurrentAttributePointArea->AddChild(CurrentAttributePointRow);
	CurrentAttributePointRow->SetPadding(FMargin(0, 0, 14, 0));
	CurrentAttributePointRow->SetCurrentGameplayTag(FRPGAuraGameplayTags::Get().Attribute_Main);
	CurrentAttributePointRow->SetWidgetController(
		UWidgetControllerBpFuncLib::CreateWidgetController(UTextValueWidgetController::StaticClass(),
		                                                   GetOwningPlayer()));

	// 创建主要属性widget
	CreateVerticalAreaWidgets(FRPGAuraGameplayTags::
	                          GetTagsContainerByType(EGameplayTagType::PrimaryGameplayTags),
	                          EGameplayTagType::PrimaryGameplayTags,
	                          VerticalBox_PrimaryAttriArea, TextValueRowButtonWidgetClass);

	// 创建次要属性widget
	VerticalBox_SecondaryAttriArea->ClearChildren();
	CreateVerticalAreaWidgets(FRPGAuraGameplayTags::
	                          GetTagsContainerByType(EGameplayTagType::SecondaryGameplayTags),
	                          EGameplayTagType::SecondaryGameplayTags,
	                          VerticalBox_SecondaryAttriArea, TextValueRowButtonWidgetClass);
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

void UAttributeMenuWidget::NativeOnInitialized()
{
	// NativeOnInitialized 先于 NativeConstruct()
	// Super::NativeOnInitialized();
	//
	// // 设置当前Widget的控制器
	// if (AttributeMenuWidgetControllerClass)
	// {
	// 	SetWidgetController(Cast<UAttributeMenuWidgetController>(
	// 			UWidgetControllerBpFuncLib::CreateWidgetController(AttributeMenuWidgetControllerClass,
	// 															   GetOwningPlayer()))
	// 	);
	// 	if (const auto Wc = GetWidgetController<UBaseWidgetController>())
	// 	{
	// 		Wc->BindCallBack();
	// 		// UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("(1)设置属性菜单控制器"))
	// 	}
	// }
	//
	// InitVerticalArea();
	// UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("(2)初始化属性菜单的Vertical区域"))
}

void UAttributeMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	//
	//
	// if(const auto Wc = GetWidgetController<UBaseWidgetController>())
	// {
	// 	Wc->BroadcastInitialValues();
	// 	// UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("(3)初始广播属性菜单控制器的委托一次"))
	// }
}


UBaseUserWidget* UAttributeMenuWidget::FindWidgetByTagFromArray(const TArray<UBaseUserWidget*>& Array,
                                                                const FGameplayTag& Tag)
{
	for (const auto Widget : Array) { if (Widget->GetCurrentGameplayTag() == Tag) { return Widget; } }
	return nullptr;
}

void UAttributeMenuWidget::CreateVerticalAreaWidgets(const FGameplayTagContainer& TagContainer,
                                                     const EGameplayTagType GameplayTagType,
                                                     UVerticalBox* VerticalArea,
                                                     const TSubclassOf<UBaseUserWidget> WidgetClass)
{
	if (TagContainer.Num() < 0 || !VerticalArea || !WidgetClass)
	{
		UE_LOG(UAttributeMenuWidgetLog, Error, TEXT("标签数组、垂直区域内容和小部件类不能为空!"));
		return;
	}
	VerticalArea->ClearChildren();
	for (const auto& GameplayTag : TagContainer)
	{
		const auto Widget = CreateWidget<UBaseUserWidget>(GetOwningPlayer(), WidgetClass);
		Widget->SetCurrentGameplayTag(GameplayTag);
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
