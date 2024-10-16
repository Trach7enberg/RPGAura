// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseSummonAbility.h"

#include "Components/CapsuleComponent.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

UBaseSummonAbility::UBaseSummonAbility()
{
	SpawnNum = 0;
	CurrentSpawnSummonIndex = 0;
	AcceptableMinRadius = 150.f;
	AcceptableMaxRadius = 450.f;
	SpawnSpread = 90.f;
}

TArray<FVector> UBaseSummonAbility::GetSpawnLocation()
{
	if (!GetAvatarActorFromActorInfo()) { return TArray<FVector>(); }

	const auto CombatInt = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (!CombatInt) { return TArray<FVector>(); }

	// 总数等于人物当前可以召唤的最大数
	SpawnNum = CombatInt->GetMaxSummonsCount() - CombatInt->GetCurrentSummonsCount();
	if (!SpawnNum) { return TArray<FVector>(); }

	TArray<FVector> Result;
	const auto ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const auto Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	TArray<FVector> TmpDir;
	URPGAuraBlueprintFunctionLibrary::GetVectorBySpread(SpawnSpread, SpawnNum, ForwardVector, TmpDir);
	for (auto& Dir : TmpDir)
	{
		const FVector RandomLocOnLine = Location + Dir * FMath::RandRange(
			AcceptableMinRadius, AcceptableMaxRadius);

		FHitResult HitResult;
		// 射线检测,使得召唤物始终在地面
		GetWorld()->LineTraceSingleByChannel(HitResult, RandomLocOnLine + FVector(0, 0, 400),
											 RandomLocOnLine - FVector(0, 0, 400), ECC_Visibility);

		if (!HitResult.bBlockingHit) { continue; }

		DrawDebug(Location, Dir, HitResult.ImpactPoint, 8, 10);
		Result.Add(HitResult.ImpactPoint);
	}
	return Result;
}

const AActor* UBaseSummonAbility::SpawnSummon(const FVector& Location, const bool bRandomChoseSummonClass)
{
	if (!SummonClasses.Num()) { return nullptr; }

	if (!GetAvatarActorFromActorInfo()) { return nullptr; }

	const auto AvatarPawn = Cast<ACharacter>(GetAvatarActorFromActorInfo());
	if (!AvatarPawn) { return nullptr; }

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = AvatarPawn;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	const int32 Index = (bRandomChoseSummonClass)
		                    ? FMath::RandRange(0, SummonClasses.Num() - 1)
		                    : CurrentSpawnSummonIndex++ % SummonClasses.Num();
	const auto SpawnActor = GetWorld()->SpawnActor<APawn>(SummonClasses[Index],
	                                                      Location + FVector(
		                                                      0, 0, AvatarPawn->GetCapsuleComponent()->
		                                                      GetScaledCapsuleHalfHeight() * 2.f),
	                                                      GetAvatarActorFromActorInfo()->GetActorForwardVector().
	                                                      Rotation(),
	                                                      SpawnParameters);
	SpawnActor->OnDestroyed.AddDynamic(this, &UBaseSummonAbility::SpawnActorOnDestroy);

	return SpawnActor;
}

void UBaseSummonAbility::DrawDebug(const FVector& Location, const FVector& TmpDeltaSpread,
                                   const FVector& RandomLocOnLine, const float RangeSphereRadius,
                                   const float SpawnSphereRadius, const float RangeDuration,
                                   const float SpawnDuration) const
{
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location,
	                                     Location + TmpDeltaSpread * AcceptableMaxRadius, 5, FLinearColor::Yellow,
	                                     RangeDuration);
	// 绘制最小最大半径调试球
	UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(),
	                                      Location + TmpDeltaSpread * AcceptableMinRadius,
	                                      RangeSphereRadius, 12, FLinearColor::Blue, RangeDuration);
	UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(),
	                                      Location + TmpDeltaSpread * AcceptableMaxRadius,
	                                      RangeSphereRadius, 12, FLinearColor::Red, RangeDuration);
	// 绘制生成点的球
	UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), RandomLocOnLine,
	                                      SpawnSphereRadius, 12, FLinearColor::Black, SpawnDuration);
}

void UBaseSummonAbility::SpawnActorOnDestroy(AActor* DestroyedActor)
{
	const auto CombatInt = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInt) { CombatInt->UpdateCurrentSummonsCount(-1); }
}
