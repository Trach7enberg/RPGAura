// Copyright GGBAO 


#include "Weapons/Projectiles/SlingshotProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/AudioComponent.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ASlingshotProjectileLog, All, All);

ASlingshotProjectile::ASlingshotProjectile()
{
	bIgnoreFriend = true;
	BIsHit = false;
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
	DamageEffectParams.TargetAbilitySystemComponent =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!DamageEffectParams.IsParamsValid())
	{
		Destroy();
		return;
	}
	
	const auto EffectCauser = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	if (!EffectCauser || EffectCauser == OtherActor) { return; }

	if (bIgnoreFriend && URPGAuraBlueprintFunctionLibrary::IsFriendly(EffectCauser, OtherActor)) { return; }
	if (!BIsHit){SpawnVfxAndSound();BIsHit = true;}


	if (HasAuthority())
	{
		URPGAuraBlueprintFunctionLibrary::ApplyDamageGameplayEffectByParams(DamageEffectParams);
		Destroy();
	}
	else
	{
		// 投射物有可能在还没有生成特效和声音的时候就被销毁了(重叠在服务器上发生,销毁的行为会复制到客户端会导致客户端还没有重叠的时候就销毁了飞弹)
		// 因此我们标记一下当前客户端的投射物已经击中了,然后重写Destroy函数,在被销毁前播放特效和声音
		BIsHit = true;
	}
}

void ASlingshotProjectile::Destroyed()
{
	if (!BIsHit && !HasAuthority()) { SpawnVfxAndSound(); }

	if (IsValid(LoopSoundAudioComponent.Get())) { LoopSoundAudioComponent->Stop(); }
	BIsHit = true;
	Super::Destroyed();
}
