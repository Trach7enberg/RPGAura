// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseArcane.h"

#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPGAura/RPGAura.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseArcaneLog, All, All);

void UBaseArcane::DetectGroundPoints(const FVector& StartCenter)
{
	const auto LocalArcaneSpawnNum = GetValidAbilityCount(GetAbilityLevel());
	auto LocalTmpCenter = StartCenter;
	LocalTmpCenter.Z += 40;
	TArray<FVector> LocalTmp{};
	const auto NewLocalSpread = (ArcaneSpread == 360.f && LocalArcaneSpawnNum == 2)
		                            ? FMath::RandRange(45, 300)
		                            : ArcaneSpread;
	URPGAuraBlueprintFunctionLibrary::GetVectorBySpread(NewLocalSpread, LocalArcaneSpawnNum, LocalTmpCenter.ForwardVector,
	                                                    LocalTmp);
	GroundPoints.Reset();

	const auto VectorMaxLen = FMath::Max(ArcaneOutRadius, 250.f);
	// 生成的点位最小值根据内半径来变化
	const auto VectorMinLen = (ArcaneInnerRadius > 0) ? ArcaneInnerRadius / 2.f : VectorMaxLen;
	for (auto& Vector : LocalTmp)
	{
		const auto RandLen = FMath::RandRange(VectorMinLen, VectorMaxLen);

		auto GroundPoint = LocalTmpCenter + Vector * RandLen;
		
		if (BShowDebugDraw)
		{
			UKismetSystemLibrary::DrawDebugArrow(this, LocalTmpCenter, LocalTmpCenter + Vector * VectorMaxLen, 1000.f,
			                                     FLinearColor::Red,
			                                     5, 5);
			UKismetSystemLibrary::DrawDebugSphere(this, GroundPoint, 10, 12, FLinearColor::Green, 5,
			                                      5);
		}
		FHitResult AdjustHitResultForVertical;
		UKismetSystemLibrary::LineTraceSingleByProfile(
			this, {GroundPoint.X, GroundPoint.Y, GroundPoint.Z + VectorMaxLen},
			{GroundPoint.X, GroundPoint.Y, GroundPoint.Z - VectorMaxLen},
			COLLISION_PRESET_DETECT_GROUND_POINTS,
			true, {}, (BShowDebugDraw)
				          ? EDrawDebugTrace::Type::ForDuration
				          : EDrawDebugTrace::Type::None,
			AdjustHitResultForVertical, true,
			FLinearColor::Blue, FLinearColor::Blue, 5);
		// TODO 待对水平向量进行光线检测修正,因为点可能会超出范围导致穿模 
		GroundPoints.Add(AdjustHitResultForVertical.ImpactPoint);
	}
}

void UBaseArcane::SpawnArcaneShards(const FVector& SpawnCenter)
{
	if (!GetWorld())
	{
		K2_EndAbility();
		return;
	}
	if (GroundPoints.Num() && GetWorld()->GetTimerManager().IsTimerPaused(ArcaneTimeHandle))
	{
		LocalPointsIndex = 0;
		// 首先在中心生成一个奥术碎片
		CauseArcaneRadiusDamage(SpawnCenter);
		if (BShowDebugDraw)
		{
			UKismetSystemLibrary::DrawDebugSphere(this, SpawnCenter, ArcaneOutRadius, 20, FLinearColor::Green,
			                                      1,
			                                      1);
			UKismetSystemLibrary::DrawDebugSphere(this, SpawnCenter, ArcaneInnerRadius, 20, FLinearColor::Green,
			                                      1,
			                                      1);
		}
		URPGAuraBlueprintFunctionLibrary::ShuffleArray(GroundPoints);
		// 再执行多重生成碎片
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
		GetWorld()->GetTimerManager().SetTimer(ArcaneTimeHandle, this, &UBaseArcane::SpawnArcaneWithDelay,
		                                       ArcaneSpawnDelay, true);
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
	if (GroundPoints.IsValidIndex(LocalPointsIndex) && GroundPoints.Num() > 1)
	{
		const auto CurrentCenter = GroundPoints[LocalPointsIndex++];

		if (BShowDebugDraw)
		{
			UKismetSystemLibrary::DrawDebugSphere(this, CurrentCenter, ArcaneOutRadius, 1,
			                                      FLinearColor::Black, 1,
			                                      2);
			UKismetSystemLibrary::DrawDebugSphere(this, CurrentCenter, ArcaneInnerRadius, 1,
			                                      FLinearColor::White, 1,
			                                      3);
		}

		CauseArcaneRadiusDamage(CurrentCenter);
	}
	else
	{
		K2_CommitAbility();
		K2_EndAbility();
	}
}

void UBaseArcane::CauseArcaneRadiusDamage(const FVector& CurrentCenter)
{
	TArray<AActor*> LocalOutOverlappingActors{};
	URPGAuraBlueprintFunctionLibrary::FindLivePlayersWithinRadius(GetAvatarActorFromActorInfo(),
	                                                              LocalOutOverlappingActors, {}, ArcaneOutRadius,
	                                                              CurrentCenter
	                                                              , true,
	                                                              FRPGAuraGameplayTags::Get().Player);
	FGameplayCueParameters Parameters{};
	Parameters.Location = CurrentCenter;
	K2_ExecuteGameplayCueWithParams(FRPGAuraGameplayTags::Get().GameplayCue_Spawn_ArcaneShards, Parameters);

	for (const auto& Actor : LocalOutOverlappingActors)
	{
		FDamageEffectParams DamageEffectParams{};
		// 设置范围伤害缩减系数
		DamageEffectParams.RadiusDamageFallOffFactor =
			URPGAuraBlueprintFunctionLibrary::GetRadialDamageWithFallOffFactor(
				Actor->GetActorLocation(), CurrentCenter, ArcaneOutRadius, ArcaneInnerRadius,
				ArcaneOutRadiusMinDamageFactor, ArcaneOutRadiusTolerance);
		// 设置冲击方向 默认从中心点向Actor冲击
		const auto Dir = (Actor->GetActorLocation() - CurrentCenter).GetSafeNormal();
		DamageEffectParams.ImpulseVector = Dir.RotateAngleAxis(45, Dir.RightVector) * KnockBackFactor *
			KnockBackDirection;

		CauseDamage(Actor, DamageEffectParams);
		UKismetSystemLibrary::DrawDebugArrow(this, CurrentCenter,
												 CurrentCenter.GetSafeNormal() + DamageEffectParams.ImpulseVector, 1000,
												 FLinearColor::White, 3, 2);
		if (BShowDebugDraw)
		{
			
			UKismetSystemLibrary::DrawDebugSphere(this, Actor->GetActorLocation(), 20, 20,
			                                      FLinearColor::Yellow, 1,
			                                      3);
			UE_LOG(UBaseArcaneLog, Warning,
			       TEXT("Actor->[%s] , "
				       "位置:[%s] , \n"
				       "检测半径[%3.1f] , "
				       "离圆心距离[%.3f] , "
				       "容差距离[%.3f] , "
				       "衰减系数:[%.3f] , "
				       "最终伤害:[%.3f]"),
			       *Actor->GetName(),
			       *Actor->GetActorLocation().ToString(),
			       ArcaneOutRadius,
			       FVector::Dist(Actor->GetActorLocation(),CurrentCenter),
			       ArcaneOutRadiusTolerance,
			       DamageEffectParams.RadiusDamageFallOffFactor,
			       DamageEffectParams.BaseDamage);
		}
	}
}

int32 UBaseArcane::GetValidAbilityCount(const int32 AbilityLevel) { 
	return FMath::Min(AbilityLevel, ArcaneSpawnNum);
 }
