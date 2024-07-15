// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UBaseWidgetController;
/**
 *  基础用户UI类
 */
UCLASS()
class RPGAURA_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="WidgetController")
	UBaseWidgetController *GetWidgetController() const { return WidgetController; }

	UFUNCTION(BlueprintCallable, Category="WidgetController")
	void SetWidgetController(UBaseWidgetController *NewWidgetController);

protected:
	virtual void NativeOnInitialized() override;

	/// 当控制器被设置时调用该事件
	UFUNCTION(BlueprintImplementableEvent, Category="WidgetController")
	void WidgetControllerSet();

private:
	TObjectPtr<UBaseWidgetController> WidgetController;
};
