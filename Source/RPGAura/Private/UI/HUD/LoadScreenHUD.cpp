// Copyright GGBAO 


#include "UI/HUD/LoadScreenHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/MVVM/ViewModel/MVVMLoadScreen.h"
#include "UI/Widgets/LoadScreenWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogLoadScreenHUD, All, All)

ALoadScreenHUD::ALoadScreenHUD() {}

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	if (!LoadScreenWidgetClass || !MvvmLoadScreenClass)
	{
		UE_LOG(LogLoadScreenHUD, Error, TEXT("[%s]HUD所需生成类为nullptr!"), *GetNameSafe(this));
		return;
	}

	CurrentLoadScreenViewModel = NewObject<UMVVMLoadScreen>(this, MvvmLoadScreenClass);

	CurrentLoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
	if (!CurrentLoadScreenWidget)
	{
		UE_LOG(LogLoadScreenHUD, Error, TEXT("[%s]无法生成LoadScreenWidget!"), *GetNameSafe(this));
		return;
	}
	
	CurrentLoadScreenWidget->InitializeLoadSlotSwitcherWidget();
	CurrentLoadScreenWidget->AddToViewport();
}
