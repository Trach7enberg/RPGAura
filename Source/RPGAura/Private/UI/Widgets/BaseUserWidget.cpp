// Copyright GGBAO 


#include "UI/Widgets/BaseUserWidget.h"

void UBaseUserWidget::NativeOnInitialized()
{
	//
	Super::NativeOnInitialized();
}

void UBaseUserWidget::SetWidgetController(UBaseWidgetController *NewWidgetController)
{
	WidgetController = NewWidgetController;
	WidgetControllerSet();
}
