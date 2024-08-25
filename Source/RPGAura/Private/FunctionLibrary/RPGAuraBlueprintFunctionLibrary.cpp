// Copyright GGBAO 


#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "Interfaces/CombatInterface.h"

DEFINE_LOG_CATEGORY_STATIC(URPGAuraBlueprintFunctionLibraryLog,All,All);

void URPGAuraBlueprintFunctionLibrary::FindLivePlayersWithinRadius(const AActor* Causer,
                                                                   TArray<AActor*>& OutOverlappingActors,
                                                                   const TArray<AActor*>& IgnoreActors, float Radius,
                                                                   const FVector& SphereOrigin,bool IgnoreSelf)
{
	if(!Causer || !IsValid(Causer))
	{
		UE_LOG(URPGAuraBlueprintFunctionLibraryLog,Warning,TEXT("Causer无效"));
		return;
	}
	
	FCollisionQueryParams SphereParams;
	if (IgnoreSelf)
	{
		SphereParams.AddIgnoredActor(Causer);
	}
	SphereParams.AddIgnoredActors(IgnoreActors);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(Causer, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
		                                FCollisionObjectQueryParams(
			                                FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), SphereParams);
	}

	for (auto& OverlapResult : Overlaps)
	{
		// 忽视友军(敌人)
		if (OverlapResult.GetActor()->ActorHasTag(FRPGAuraGameplayTags::Get().Enemy)) { continue; }

		const auto DoseImplement = OverlapResult.GetActor()->Implements<UCombatInterface>();
		if (!DoseImplement) { continue; }

		const auto CombatInterface = Cast<ICombatInterface>(OverlapResult.GetActor());
		if (!CombatInterface) { continue; }

		const auto IsCharacterDie = CombatInterface->IsCharacterDie();
		if (!IsCharacterDie) { OutOverlappingActors.AddUnique(OverlapResult.GetActor()); }
	}
}
