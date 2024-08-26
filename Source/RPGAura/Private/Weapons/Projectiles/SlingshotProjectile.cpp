// Copyright GGBAO 


#include "Weapons/Projectiles/SlingshotProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ASlingshotProjectileLog, All, All);

ASlingshotProjectile::ASlingshotProjectile()
{
	bIgnoreFriendly = true;
	ProjectileMovementComponent->InitialSpeed = 1500.f;
	ProjectileMovementComponent->MaxSpeed = 1500.f;
	ProjectileMovementComponent->ProjectileGravityScale = 1.f;
}

void ASlingshotProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileLifeSpawn = 3.f;
}

void ASlingshotProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	const auto EffectCauser = DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser();
	if (!EffectCauser || !DamageEffectSpecHandle.IsValid() || EffectCauser ==
		OtherActor) { return; }

	if (URPGAuraBlueprintFunctionLibrary::IsFriendly(EffectCauser, OtherActor)) { return; }

	if (HasAuthority())
	{
		UAbilitySystemComponent* ActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

		if (!DamageEffectSpecHandle.IsValid() || !ActorAsc)
		{
			// UE_LOG(ASlingshotProjectileLog, Warning, TEXT("[%s]无法应用GE!"), *GetName());
		}
		else { ActorAsc->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, ActorAsc); }
		Destroy();
	}
}
