// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

UBaseSummonAbility::UBaseSummonAbility()
{
	SpawnNum = 5;

	AcceptableMinRadius = 150.f;
	AcceptableMaxRadius = 450.f;
	SpawnSpread = 90.f;
}

TArray<FVector> UBaseSummonAbility::GetSpawnLocation()
{
	if (!GetAvatarActorFromActorInfo()) { return TArray<FVector>{}; }

	TArray<FVector> Result;
	const auto ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const auto Location = GetAvatarActorFromActorInfo()->GetActorLocation();

	// 扇形区域内的一个个小角度,用于生成召唤物
	const auto DeltaSpread = SpawnSpread / SpawnNum;


	const auto LeftOfSpread = ForwardVector.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);

	const auto RightOfSpread = ForwardVector.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);

	// 绘制夹角调试线


	for (int i = 0; i < SpawnNum; ++i)
	{
		const auto TmpDeltaSpread = LeftOfSpread.RotateAngleAxis(i * DeltaSpread, FVector::UpVector);
		const FVector RandomLocOnLine = Location + TmpDeltaSpread * FMath::RandRange(
			AcceptableMinRadius, AcceptableMaxRadius);
		
		FHitResult HitResult;
		// 射线检测,使得召唤物始终在地面
		GetWorld()->LineTraceSingleByChannel(HitResult,RandomLocOnLine + FVector(0,0,400),RandomLocOnLine - FVector(0,0,400),ECC_Visibility);

		if(!HitResult.bBlockingHit){continue;}
		
		DrawDebug(Location, TmpDeltaSpread, HitResult.ImpactPoint,8,10);
		Result.Add(HitResult.ImpactPoint);
	}
	
	return Result;
}

void UBaseSummonAbility::DrawDebug(const FVector& Location, const FVector& TmpDeltaSpread,
                                   const FVector& RandomLocOnLine, const float RangeSphereRadius,
                                   const float SpawnSphereRadius, const float RangeDuration , const float SpawnDuration ) const
{
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), Location,
	                                     Location + TmpDeltaSpread * AcceptableMaxRadius, 5, FLinearColor::Yellow, RangeDuration);
	// 绘制最小最大半径调试球
	UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(),
	                                      Location + TmpDeltaSpread * AcceptableMinRadius,
	                                      RangeSphereRadius, 12, FLinearColor::Red, RangeDuration);
	UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(),
	                                      Location + TmpDeltaSpread * AcceptableMaxRadius,
	                                      RangeSphereRadius, 12, FLinearColor::Red, RangeDuration);
	// 绘制生成点的球
	UKismetSystemLibrary::DrawDebugSphere(GetAvatarActorFromActorInfo(), RandomLocOnLine,
	                                      SpawnSphereRadius, 12, FLinearColor::Black, SpawnDuration);
}
