// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class UBehaviorTreeComponent;
/**
 *  AI 控制器基类
 */
UCLASS()
class RPGAURA_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABaseAIController();
	virtual void OnPossess(APawn* InPawn) override;
protected:

	// 行为树组件
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
