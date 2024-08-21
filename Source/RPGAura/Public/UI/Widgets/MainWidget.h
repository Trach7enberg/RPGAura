// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "MainWidget.generated.h"

class UAttributeMenuWidget;
/**
 * 主widget
 */
UCLASS()
class RPGAURA_API UMainWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	// 属性菜单
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UAttributeMenuWidget> AttributeMenu;
};
