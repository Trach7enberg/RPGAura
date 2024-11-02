// Copyright GGBAO 


#include "GAS/GameplayAbilities/BlazeBurstProjectileAbility.h"

#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/Projectiles/BaseProjectile.h"
#include "Weapons/Projectiles/BlazeBurstProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(UBlazeBurstProjectileAbilityLog, All, All);


void UBlazeBurstProjectileAbility::SpawnProjectilesAround(const FVector& Center, const bool bOverridePitch,
                                                          const float OverridePitch)
{
	// 飞弹能力(投射物)仅在服务器上生成,世界正在摧毁也不能生成
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->HasAuthority() || !GetWorld()) { return; }

	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBlazeBurstProjectileAbilityLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}

	const auto Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!Instigator) { return; }

	auto ForwardRotator = GetAvatarActorFromActorInfo()->GetActorForwardVector().Rotation();
	if (bOverridePitch) { ForwardRotator.Pitch = OverridePitch; }
	TArray<FVector> Directions;
	URPGAuraBlueprintFunctionLibrary::GetVectorBySpread(SpawnSpread, GetValidAbilityCount(GetAbilityLevel()),
	                                                    ForwardRotator.Vector(),
	                                                    Directions);

	const auto& CurrentCenter = Center;
	SpawnImpulsiveAtLoc(CurrentCenter, AroundRadius);
	for (auto& ProjDirection : Directions)
	{
		// UKismetSystemLibrary::DrawDebugArrow(this, CurrentCenter, CurrentCenter + ProjDirection * 1000.f, 1000.f,
		//                                      FLinearColor::Red, 3, 2);
		FTransform SpawnTrans;
		SpawnTrans.SetLocation(CurrentCenter);
		SpawnTrans.SetRotation(ProjDirection.Rotation().Quaternion());
		
		const auto Projectile = Cast<ABlazeBurstProjectile>(CreateProjectile(SpawnTrans, Instigator));
		if (!Projectile) { continue; }
		const auto ProjComp = Projectile->GetProjectileMovementComponent();
		if (!Projectile) { continue; }
		
		ProjComp->ProjectileGravityScale = ProjectileGravity;
		ProjComp->InitialSpeed = AroundBurstSpeed;
		ProjComp->MaxSpeed = AroundBurstSpeed;
		if (bIsHomingProjectile)
		{
			Projectile->SetOwner(GetAvatarActorFromActorInfo());
			Projectile->
				EnableHomingActorWithDelay(FollowDelay,  FMath::FRandRange(HomingMinAcceleration, HomingMaxAcceleration));
		}
		Projectile->FinishSpawning(SpawnTrans);
	}
}

void UBlazeBurstProjectileAbility::SpawnImpulsiveAtLoc(const FVector& Center, const float Radius)
{
	TArray<AActor*> Actors{};
	URPGAuraBlueprintFunctionLibrary::FindLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), Actors, {}, Radius,
	                                                              Center, true, FRPGAuraGameplayTags::Get().Player);
	// UKismetSystemLibrary::DrawDebugSphere(this, Center, Radius, 14, FLinearColor::Red, 1, 2);
	for (const auto& Actor : Actors)
	{
		const auto Dir = (Actor->GetActorLocation() - Center).GetSafeNormal();
		const auto CombIntF = Cast<ICombatInterface>(Actor);
		if (!CombIntF) { continue; }
		// TODO 待修改当前技能造成的冲击力如果是致命伤害可以使敌人腾空击飞..   
		CombIntF->AddKnockBack(Dir * KnockBackFactor * KnockBackDirection);
	}
}

int32 UBlazeBurstProjectileAbility::GetValidAbilityCount(const int32 AbilityLevel) { return MaxProjectileNum; }
