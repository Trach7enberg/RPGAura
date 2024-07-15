// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

/**
 * 基础玩家控制类
 */
UCLASS()
class RPGAURA_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ABasePlayerController();
	
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	

	
private:
	/// 输入上下文
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> InputContext;

	//IA_Move
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue &InputActionValue);

};
