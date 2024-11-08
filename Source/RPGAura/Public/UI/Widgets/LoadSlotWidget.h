// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "LoadSlotWidget.generated.h"

class UMVVMLoadSlot;
/**
 * 加载关卡的加载选项槽widget
 */
UCLASS()
class RPGAURA_API ULoadSlotWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	/// 处于初始化LoadSlotWidget时的事件
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void OnInitializeLoadSlotWidget();
	
};
