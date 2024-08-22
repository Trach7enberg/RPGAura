// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_Attack.generated.h"

/**
 *  攻击任务类
 */
UCLASS()
class RPGAURA_API UBTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
