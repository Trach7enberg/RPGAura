// Copyright GGBAO 


#include "UI/HUD/BaseHUD.h"

#include "FunctionLibrary/WidgetControllerBpFuncLib.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "UI/WidgetControllers/MainWidgetController.h"

DEFINE_LOG_CATEGORY_STATIC(ABaseHUDLog, All, All);

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();
}


UMainWidgetController *ABaseHUD::GetMainWidgetController() const
{

	const auto Result = Cast<UMainWidgetController>(CurrentMainWidget->GetWidgetController());
	if (!Result)
	{
		UE_LOG(ABaseHUDLog, Error, TEXT("获取 MainWidgetController 失败!"));
		return nullptr;
	}

	return Result;
}

void ABaseHUD::InitHudMainWidget()
{
	if (!GetOwningPlayerController() || !MainWidgetClass)
	{
		return;
	}

	if (CurrentMainWidget)
	{
		return;
	}

	CurrentMainWidget = Cast<UBaseUserWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), MainWidgetClass));
	if (!CurrentMainWidget)
	{
		return;
	}

	// 给当前主Widget设置控制器
	CurrentMainWidget->SetWidgetController(
		UWidgetControllerBpFuncLib::CreateWidgetController(MainWidgetControllerClass, GetOwningPlayerController()));

	if (!CurrentMainWidget->GetWidgetController())
	{
		UE_LOG(ABaseHUDLog, Error, TEXT("创建 MainWidgetController 失败!"));
		return;
	}


	CurrentMainWidget->GetWidgetController()->BroadcastInitialValues();

	// TODO 把蓝图中设置的属性菜单换到这里统一创建属性菜单,并且设置属性菜单的控制器、BroadcastInitialValues();

	CurrentMainWidget->AddToViewport();
}
