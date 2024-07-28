// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "RPGAuraCoreTypes.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 *  Widget控制器需要的基本参数的结构体
 */
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams()
	{
	}

	FWidgetControllerParams(APlayerController *Pc,
							APlayerState *Ps,
							UAbilitySystemComponent *Asc,
							UAttributeSet *As) :
		CurrentPlayerController(Pc), CurrentPlayerState(Ps),
		CurrentAbilitySystemComponent(Asc), CurrentAttributeSet(As)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> CurrentPlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> CurrentPlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> CurrentAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> CurrentAttributeSet = nullptr;
};

