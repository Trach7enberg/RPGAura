// Copyright GGBAO 


#include "UI/HUD/BaseHUD.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "Blueprint/UserWidget.h"

void ABaseHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwningPlayerController() || !MainWidgetClass) { return; }

	CurrentMainWidget = Cast<UBaseUserWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), MainWidgetClass));
	if (!CurrentMainWidget) { return; }

	CurrentMainWidget->AddToViewport();
}
