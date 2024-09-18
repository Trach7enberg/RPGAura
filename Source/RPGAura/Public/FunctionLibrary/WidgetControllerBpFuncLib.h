// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Components/PanelWidget.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "UI/Widgets/BaseUserWidget.h"

#include "WidgetControllerBpFuncLib.generated.h"

class UBaseUserWidget;
class UPanelWidget;
struct FWidgetControllerParams;
class UBaseWidgetController;
/**
 * Widget控制器的蓝图函数库
 */
UCLASS()
class RPGAURA_API UWidgetControllerBpFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UWidgetControllerBpFuncLib();

	/// 获取创建Widget控制器所需要的基本参数
	/// @param OwnerController Widget控制器的拥有者(得是玩家控制器!)
	/// @param Params 控制器的基本参数
	UFUNCTION(BlueprintPure, Category="WidgetController")
	static void CreateWidgetControllerParams(AController* OwnerController, FWidgetControllerParams& Params);

	/// 创建Widget的控制器 
	/// @param WidgetControllerClass 控制器的Class
	/// @param OwnerController 控制器的Owner(得是玩家控制器!)
	/// @return T类型的Widget控制器
	UFUNCTION(BlueprintPure, Category="WidgetController")
	static UBaseWidgetController* CreateWidgetController(TSubclassOf<UBaseWidgetController> WidgetControllerClass,
	                                                     AController* OwnerController);

	/// 在给定的区域(垂直盒或者水平盒)创建多个重复的Widget
	/// @tparam BoxSlotType 垂直盒或者水平盒的Slot(VerticalBoxSlot、HorizontalBoxSlot)
	/// @param WidgetClass 要创建的Widget的class
	/// @param WidgetOwner widget的拥有者
	/// @param Area 给定的区域(垂直盒或者水平盒)
	/// @param Num 创建重复Widget的数量
	/// @param WidgetControllerClass 要创建Widget的小部件控制器
	/// @param TagToAbilityInfos
	/// @param HorizontalAlignment BoxSlot的水平对齐格式
	/// @param VerticalAlignment BoxSlot的垂直对齐格式
	template <typename BoxSlotType>
	static void CreateMultipleWidget(const TSubclassOf<UBaseUserWidget> WidgetClass, APlayerController* WidgetOwner,
	                                 UPanelWidget* Area, const int32 Num,
	                                 TArray<FTagToAbilityInfo>& TagToAbilityInfos,
	                                 const TSubclassOf<UBaseWidgetController> WidgetControllerClass =
		                                 UBaseWidgetController::StaticClass(),
	                                 EHorizontalAlignment HorizontalAlignment = HAlign_Fill,
	                                 EVerticalAlignment VerticalAlignment = VAlign_Fill);
};

template <typename BoxSlotType>
void UWidgetControllerBpFuncLib::CreateMultipleWidget(const TSubclassOf<UBaseUserWidget> WidgetClass,
                                                      APlayerController* WidgetOwner, UPanelWidget* Area,
                                                      const int32 Num,
                                                      TArray<FTagToAbilityInfo>& TagToAbilityInfos,
                                                      const TSubclassOf<UBaseWidgetController> WidgetControllerClass,
                                                      EHorizontalAlignment HorizontalAlignment,
                                                      EVerticalAlignment VerticalAlignment)
{
	if (!WidgetClass || !WidgetControllerClass)
	{
		UE_LOG(LogTemp, Error, TEXT("给定的资源为空!"));
		return;
	}
	if (!Area || !WidgetOwner) { return; }

	Area->ClearChildren();
	for (int i = 0; i < Num; ++i)
	{
		const auto Widget = CreateWidget<UBaseUserWidget>(WidgetOwner, WidgetClass);
		if (!Widget) { continue; }

		// 设置Widget的游戏标签(能力相对应的输入键)
		if (i < TagToAbilityInfos.Num()) { Widget->SetCurrentGameplayTag(TagToAbilityInfos[i].InputTag); }
		
		Widget->SetWidgetController(CreateWidgetController(WidgetControllerClass,
																   WidgetOwner));

		Area->AddChild(Widget);

		// 获取widget身上(父类的垂直框Slot)
		const auto WidgetBoxSlot = Cast<BoxSlotType>(Widget->Slot.Get());
		if (!WidgetBoxSlot) { continue; }

		WidgetBoxSlot->SetHorizontalAlignment(HorizontalAlignment);
		WidgetBoxSlot->SetVerticalAlignment(VerticalAlignment);
	}
}
