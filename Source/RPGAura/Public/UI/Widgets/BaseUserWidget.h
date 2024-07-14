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
	UBaseWidgetController *GetWidgetController() const { return WidgetController; }

	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UBaseWidgetController *NewWidgetController);

protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UBaseWidgetController> WidgetController;

	virtual void NativeOnInitialized() override;

	/// 当控制器被设置时调用该事件
	UFUNCTION(BlueprintImplementableEvent, Category="WidgetController")
	void WidgetControllerSet();
};
