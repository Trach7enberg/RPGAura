// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseArcane.h"

#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UBaseArcane::DetectGroundPoints(const FVector& StartCenter, const float Radius, const float Spread,
                                     const int32 Points)
{
	auto LocalTmpCenter = StartCenter;
	LocalTmpCenter.Z += 40;
	TArray<FVector> LocalTmp{};
	const auto NewLocalSpread = (Spread == 360.f && Points == 2) ? FMath::RandRange(45, 300) : Spread;
	URPGAuraBlueprintFunctionLibrary::GetVectorBySpread(NewLocalSpread, Points, LocalTmpCenter.ForwardVector, LocalTmp);
	GroundPoints.Reset();
	const auto VectorLen = FMath::Max(Radius, 250.f);

	for (auto& Vector : LocalTmp)
	{
		const auto RandLen = FMath::RandRange(100.f, VectorLen);
		UKismetSystemLibrary::DrawDebugArrow(this, LocalTmpCenter, LocalTmpCenter + Vector * VectorLen, 1000.f,
		                                     FLinearColor::Red,
		                                     5, 5);
		auto GroundPoint = LocalTmpCenter + Vector * RandLen;
		UKismetSystemLibrary::DrawDebugSphere(this, GroundPoint, 10, 12, FLinearColor::Green, 5,
		                                      5);
		FHitResult HitResult;
		UKismetSystemLibrary::LineTraceSingleByProfile(this, {GroundPoint.X, GroundPoint.Y, GroundPoint.Z + VectorLen},
		                                               {GroundPoint.X, GroundPoint.Y, GroundPoint.Z - VectorLen},
		                                               "DetectGroundPoints",
		                                               true, {}, EDrawDebugTrace::Type::ForDuration, HitResult, true,
		                                               FLinearColor::Blue, FLinearColor::Blue, 5);
		// TODO 待对水平向量进行光线检测修正,因为点可能会超出范围导致穿模 
		GroundPoints.Add(HitResult.ImpactPoint);
	}
}

void UBaseArcane::SpawnArcaneShards(const FVector& SpawnCenter)
{
	if (!GetWorld()) { return; }
	if (GroundPoints.Num() && GetWorld()->GetTimerManager().IsTimerPaused(ArcaneTimeHandle))
	{
		LocalPointsIndex = 0;
		// 首先在中心生成一个奥术碎片
		FGameplayCueParameters Parameters{};
		Parameters.Location = SpawnCenter;
		K2_ExecuteGameplayCueWithParams(FRPGAuraGameplayTags::Get().GameplayCue_Spawn_ArcaneShards, Parameters);
		// 再执行多重生成碎片
		URPGAuraBlueprintFunctionLibrary::ShuffleArray(GroundPoints);
		GetWorld()->GetTimerManager().UnPauseTimer(ArcaneTimeHandle);
	}
}

void UBaseArcane::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                  const FGameplayAbilityActivationInfo ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 初始化定时器
	if (GetWorld() && !GetWorld()->GetTimerManager().IsTimerActive(ArcaneTimeHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(ArcaneTimeHandle, this, &UBaseArcane::SpawnArcaneWithDelay, 0.2f, true);
		GetWorld()->GetTimerManager().PauseTimer(ArcaneTimeHandle);
	}
}

void UBaseArcane::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                             const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility,
                             bool bWasCancelled)
{
	// 清除定时器
	if (GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(ArcaneTimeHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(ArcaneTimeHandle);
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBaseArcane::SpawnArcaneWithDelay()
{
	if (!GetWorld()) { return; }
	if (GroundPoints.IsValidIndex(LocalPointsIndex))
	{
		FGameplayCueParameters Parameters{};
		Parameters.Location = GroundPoints[LocalPointsIndex++];
		K2_ExecuteGameplayCueWithParams(FRPGAuraGameplayTags::Get().GameplayCue_Spawn_ArcaneShards, Parameters);
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(ArcaneTimeHandle);
		K2_EndAbility();
	}
}
