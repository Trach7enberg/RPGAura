// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindClosestCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPGAURA_API UBTService_FindClosestCharacter : public UBTService_BlueprintBase
{
	GENERATED_BODY()

public:
	UBTService_FindClosestCharacter();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BlackBoardKey")
	FBlackboardKeySelector TargetActorSelector;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="BlackBoardKey")
	FBlackboardKeySelector TargetDistanceSelector;
};
