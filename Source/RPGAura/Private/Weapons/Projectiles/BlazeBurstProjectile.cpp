// Copyright GGBAO 


#include "Weapons/Projectiles/BlazeBurstProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ABlazeBurstProjectileLog, All, All);

void ABlazeBurstProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (GetWorld() && GetWorldTimerManager().IsTimerPaused(BlazeBurstTimerHandle))
	{
		GetWorldTimerManager().UnPauseTimer(BlazeBurstTimerHandle);
	}
}

void ABlazeBurstProjectile::Destroyed()
{
	if (GetWorld()) { GetWorldTimerManager().ClearTimer(BlazeBurstTimerHandle); }
	Super::Destroyed();
}

void ABlazeBurstProjectile::EnableHomingActorWithDelay(const float Delay, const float HomingAcceleration, const AActor* HomingActor)
{
	if (!GetWorld()) { return; }

	SphereComponent.Get()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	if (GetProjectileMovementComponent())
	{
		const auto Ht = (HomingActor) ? HomingActor : GetOwner();
		GetProjectileMovementComponent()->HomingTargetComponent = (Ht) ? Ht->GetRootComponent() : nullptr;
		GetProjectileMovementComponent()->HomingAccelerationMagnitude = HomingAcceleration;
	}

	GetWorldTimerManager().SetTimer(BlazeBurstTimerHandle, this, &ABlazeBurstProjectile::BlazeBurstTimerCallback,
	                                Delay, false);
	GetWorldTimerManager().PauseTimer(BlazeBurstTimerHandle);
}

void ABlazeBurstProjectile::BlazeBurstTimerCallback()
{
	if (!GetProjectileMovementComponent() || !GetOwner() || !SphereComponent) { return; }

	if (GetProjectileMovementComponent()->HomingTargetComponent.Get())
	{
		EnableHomingActorOnDestroyedEvent();
		GetProjectileMovementComponent()->bIsHomingProjectile = true;
	}

	// 碰撞必须在最后设置,否则GetProjectileMovementComponent会有几率被Overlap导致空指针
	SphereComponent.Get()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}
