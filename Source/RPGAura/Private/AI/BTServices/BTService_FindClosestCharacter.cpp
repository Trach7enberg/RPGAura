// Copyright GGBAO 


#include "AI/BTServices/BTService_FindClosestCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

UBTService_FindClosestCharacter::UBTService_FindClosestCharacter() { NodeName = "FindClosestCharacter"; }

void UBTService_FindClosestCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(this, FRPGAuraGameplayTags::Get().Player, ActorsWithTag);

	const auto AiPawn = AIOwner->GetPawn();
	
	
	auto NumericLimits = TNumericLimits<float>();
	float ClosestDistance = NumericLimits.Max();
	AActor* ClosestActor = nullptr;
	for (const auto Actor : ActorsWithTag)
	{
		const auto CombatInterface = Cast<ICombatInterface>(Actor);
		if(!CombatInterface || CombatInterface->IsCharacterDie()){continue;} 
		if (!IsValid(Actor) || !IsValid(AiPawn)) { continue; }

		const auto Dist = AiPawn->GetDistanceTo(Actor);
		if (Dist < ClosestDistance)
		{
			ClosestDistance = Dist;
			ClosestActor = Actor;
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetActorSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, TargetDistanceSelector, ClosestDistance);
}
