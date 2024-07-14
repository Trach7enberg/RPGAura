// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 *  基础UI控制器,用来获得数据以传递给显示的UI
 */
UCLASS()
class RPGAURA_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> CurrentPlayerController;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> CurrentPlayerState;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> CurrentAbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> CurrentAttributeSet;
};
