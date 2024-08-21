// Copyright GGBAO 


#include "FunctionLibrary/WidgetControllerBpFuncLib.h"

#include "PlayerStates/BasePlayerState.h"
#include "UI/WidgetControllers/BaseWidgetController.h"


DEFINE_LOG_CATEGORY_STATIC(UWidgetControllerBpFuncLibLog, All, All);


UWidgetControllerBpFuncLib::UWidgetControllerBpFuncLib()
{
	
}

void UWidgetControllerBpFuncLib::CreateWidgetControllerParams(AController* OwnerController,
                                                              FWidgetControllerParams& Params)
{
	if (!OwnerController)
	{
		UE_LOG(UWidgetControllerBpFuncLibLog, Error, TEXT("当前Owner为空"));
		return;
	}

	const auto CurrentPlayerController = Cast<APlayerController>(OwnerController);

	if (!CurrentPlayerController)
	{
		UE_LOG(UWidgetControllerBpFuncLibLog, Error, TEXT("当前Owner不是玩家控制器!"));
		return;
	}

	const auto MyPlayerState = CurrentPlayerController->GetPlayerState<ABasePlayerState>();
	if (!MyPlayerState) { return; }

	if (!MyPlayerState->GetAbilitySystemComponent() || !MyPlayerState->GetAttributeSet()) { return; }

	Params = {
		CurrentPlayerController, MyPlayerState,
		MyPlayerState->GetAbilitySystemComponent(),
		MyPlayerState->GetAttributeSet()
	};
}


UBaseWidgetController* UWidgetControllerBpFuncLib::CreateWidgetController(
	const TSubclassOf<UBaseWidgetController> WidgetControllerClass,
	AController* OwnerController)
{
	if (!WidgetControllerClass) { return nullptr; }

	UBaseWidgetController* WidgetController = nullptr;

	auto Params = FWidgetControllerParams();
	UWidgetControllerBpFuncLib::CreateWidgetControllerParams(OwnerController, Params);

	WidgetController = NewObject<UBaseWidgetController>(OwnerController, WidgetControllerClass);
	WidgetController->SetWidgetControllerParams(Params);
	if (!WidgetController->IsWidgetControllerParamsValid())
	{
		UE_LOG(UWidgetControllerBpFuncLibLog, Error, TEXT("WidgetControllerParams 不能为 null"));
	}
	// // 给这个控制器的拥有者绑定当属性值更改时候的回调函数
	// WidgetController->BindCallBack();

	if (!WidgetController)
	{
		UE_LOG(UWidgetControllerBpFuncLibLog, Error, TEXT("[%s]类型的WidgetController 创建失败"),
		       *WidgetControllerClass->GetName());
	}

	return WidgetController;
}
