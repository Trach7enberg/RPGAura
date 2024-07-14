// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UBaseUserWidget;
/**
 * HUD类
 */
UCLASS()
class RPGAURA_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	TObjectPtr<UBaseUserWidget> CurrentMainWidget;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WidgetClass")
	TSubclassOf<UBaseUserWidget> MainWidgetClass;

};
