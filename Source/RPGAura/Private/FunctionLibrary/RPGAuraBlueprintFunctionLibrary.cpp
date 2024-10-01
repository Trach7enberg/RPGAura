// Copyright GGBAO 


#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Data/AbilityDescriptionAsset.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "Interfaces/CombatInterface.h"

DEFINE_LOG_CATEGORY_STATIC(URPGAuraBlueprintFunctionLibraryLog, All, All);

TObjectPtr<UAbilityDescriptionAsset> URPGAuraBlueprintFunctionLibrary::AbilityDescriptionAsset;

URPGAuraBlueprintFunctionLibrary::URPGAuraBlueprintFunctionLibrary() { LoadAbilityDescriptionAsset(this); }

void URPGAuraBlueprintFunctionLibrary::FindLivePlayersWithinRadius(const AActor* Causer,
                                                                   TArray<AActor*>& OutOverlappingActors,
                                                                   const TArray<AActor*>& IgnoreActors, float Radius,
                                                                   const FVector& SphereOrigin, bool IgnoreSelf)
{
	if (!Causer || !IsValid(Causer))
	{
		UE_LOG(URPGAuraBlueprintFunctionLibraryLog, Warning, TEXT("Causer无效"));
		return;
	}

	FCollisionQueryParams SphereParams;
	if (IgnoreSelf) { SphereParams.AddIgnoredActor(Causer); }
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

bool URPGAuraBlueprintFunctionLibrary::IsFriendly(const AActor* Actor1, AActor* Actor2)
{
	if (!Actor1 || !Actor2) { return false; }
	const auto bBothPlayers = Actor1->ActorHasTag(FRPGAuraGameplayTags::Get().Player) && Actor2->ActorHasTag(
		FRPGAuraGameplayTags::Get().Player);
	const auto bBothEnemies = Actor1->ActorHasTag(FRPGAuraGameplayTags::Get().Enemy) && Actor2->ActorHasTag(
		FRPGAuraGameplayTags::Get().Enemy);

	return bBothPlayers || bBothEnemies;
}

UAbilityDescriptionAsset* URPGAuraBlueprintFunctionLibrary::GetAbilityDescriptionAsset()
{
	return AbilityDescriptionAsset.Get();
}

UAbilityDescriptionAsset* URPGAuraBlueprintFunctionLibrary::GetAbilityDescriptionAsset(UObject* Outer)
{
	LoadAbilityDescriptionAsset(Outer);
	return AbilityDescriptionAsset.Get();
}

FAbilityDescription URPGAuraBlueprintFunctionLibrary::GetAbilityDescription(
	AActor* Actor, UPARAM(ref) const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid() || !Actor) { return FAbilityDescription(); }
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));

	if (!MyAsc) { return FAbilityDescription(); }

	return MyAsc->GetAbilityDescriptionByAbilityTag(AbilityTag);
}


void URPGAuraBlueprintFunctionLibrary::LoadAbilityDescriptionAsset(UObject* Outer)
{
	AbilityDescriptionAsset = LoadObject<UAbilityDescriptionAsset>(
		Outer,TEXT(
			"/Script/RPGAura.AbilityDescriptionAsset'/Game/Blueprints/GAS/Data/DataAssets/DA_AbilitiesDescriptions.DA_AbilitiesDescriptions'"));
	if (!AbilityDescriptionAsset) { UE_LOG(URPGAuraBlueprintFunctionLibraryLog, Error, TEXT("获取技能描述数据资产失败!")); }
}
