// Copyright GGBAO 


#include "Weapons/Projectiles/SpellProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(ASpellProjectileLog, All, All);

ASpellProjectile::ASpellProjectile()
{
	FireBoltNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");

	FireBoltNiagaraComponent->SetupAttachment(GetRootComponent());
}

void ASpellProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(ProjectileLifeSpawn);
	if (LoopingSound)
	{
		LoopSoundAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
}


void ASpellProjectile::SpawnVfxAndSound() const
{
	if (!ImpactSound || !ImpactEffect) { return; }
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}

void ASpellProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
	if (!DamageEffectSpecHandle.IsValid() || DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() ==
		OtherActor) { return; }

	if (!BIsHit) { SpawnVfxAndSound(); }


	UE_LOG(ASpellProjectileLog, Warning, TEXT("OverLapActor:[%s]"), *OtherActor->GetName());
	if (HasAuthority())
	{
		UAbilitySystemComponent* ActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

		if (!DamageEffectSpecHandle.IsValid() || !ActorAsc) { UE_LOG(ASpellProjectileLog, Warning, TEXT("无法应用GE!")); }
		else
		{
			// 应用GE到自身,这里的自身是OtherActor
			ActorAsc->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, ActorAsc);
		}

		Destroy();
	}
	else
	{
		// 投射物有可能在还没有生成特效和声音的时候就被销毁了(重叠在服务器上发生,销毁的行为会复制到客户端会导致客户端还没有重叠的时候就销毁了飞弹)
		// 因此我们标记一下当前客户端的投射物已经击中了,然后重写Destroy函数,在被销毁前播放特效和声音
		BIsHit = true;
	}
}

void ASpellProjectile::Destroyed()
{
	if (!BIsHit && !HasAuthority()) { SpawnVfxAndSound(); }

	if (IsValid(LoopSoundAudioComponent.Get())) { LoopSoundAudioComponent->Stop(); }
	Super::Destroyed();
}
