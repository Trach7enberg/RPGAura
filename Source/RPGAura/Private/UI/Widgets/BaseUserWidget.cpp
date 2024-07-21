// Copyright GGBAO 


#include "UI/Widgets/BaseUserWidget.h"

#include "Controller/BasePlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseUserWidgetLog, All, All);

void UBaseUserWidget::NativeOnInitialized() { Super::NativeOnInitialized(); }

void UBaseUserWidget::SetWidgetController(UBaseWidgetController *NewWidgetController)
{
	
	WidgetController = NewWidgetController;
	WidgetControllerSet();
}
