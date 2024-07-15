// Copyright GGBAO 


#include "UI/HUD/BaseHUD.h"

#include "PlayerStates/BasePlayerState.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "UI/WidgetControllers/MainWidgetController.h"

DEFINE_LOG_CATEGORY_STATIC(ABaseHUDLog, All, All);

void ABaseHUD::BeginPlay() { Super::BeginPlay(); }

void ABaseHUD::CreateWidgetControllerParams(FWidgetControllerParams &Params) const
{
	if (!GetOwningPlayerController()) { return; }

	const auto MyPlayerState = GetOwningPlayerController()->GetPlayerState<ABasePlayerState>();
	if (!MyPlayerState) { return; }

	if (!MyPlayerState->GetAbilitySystemComponent() || !MyPlayerState->GetAttributeSet()) { return; }

	Params = {GetOwningPlayerController(), MyPlayerState,
	          MyPlayerState->GetAbilitySystemComponent(),
	          MyPlayerState->GetAttributeSet()};

}

UMainWidgetController *ABaseHUD::CreateMainWidgetController()
{

	if (!MainWidgetControllerClass) { return nullptr; }

	auto MWidgetController = Cast<UMainWidgetController>(CurrentMainWidget->GetWidgetController());
	if (!MWidgetController)
	{
		auto Params = FWidgetControllerParams();
		CreateWidgetControllerParams(Params);


		MWidgetController = NewObject<UMainWidgetController>(this, MainWidgetControllerClass);
		MWidgetController->SetWidgetControllerParams(Params);
		if (!MWidgetController->IsWidgetControllerParamsValid())
		{
			UE_LOG(ABaseHUDLog, Error, TEXT("WidgetControllerParams 不能为 null"));
		}

		if (!MWidgetController) { UE_LOG(ABaseHUDLog, Error, TEXT("MainWidgetController 不能为 null")); }
	}


	return MWidgetController;
}

UMainWidgetController *ABaseHUD::GetMainWidgetController() const
{

	const auto Result = Cast<UMainWidgetController>(CurrentMainWidget->GetWidgetController());
	if (!Result)
	{
		UE_LOG(ABaseHUDLog, Error, TEXT("创建 MainWidgetController 失败!"));
		return nullptr;
	}

	return Result;
}

void ABaseHUD::InitHudMainWidget()
{
	if (!GetOwningPlayerController() || !MainWidgetClass) { return; }

	if (CurrentMainWidget) { return; }

	CurrentMainWidget = Cast<UBaseUserWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), MainWidgetClass));
	if (!CurrentMainWidget) { return; }

	CurrentMainWidget->SetWidgetController(CreateMainWidgetController());

	if (!CurrentMainWidget->GetWidgetController()) { return; }
	
	
	CurrentMainWidget->GetWidgetController()->BroadcastInitialValues();

	CurrentMainWidget->AddToViewport();
}
