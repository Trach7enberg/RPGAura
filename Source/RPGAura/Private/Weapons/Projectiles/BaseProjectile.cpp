// Copyright GGBAO 


#include "Weapons/Projectiles/BaseProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPGAura/RPGAura.h"

DEFINE_LOG_CATEGORY_STATIC(ABaseProjectileLog, All, All);

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	FireBoltNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	SetRootComponent(SphereComponent);
	FireBoltNiagaraComponent->SetupAttachment(GetRootComponent());

	SphereComponent->SetCollisionProfileName(COLLISION_PRESET_PROJECTILE);
}


void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnSphereOverlap);

	SetLifeSpan(ProjectileLifeSpawn);
	if (LoopingSound)
	{
		LoopSoundAudioComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
}

void ABaseProjectile::SpawnVfxAndSound() const
{
	if (!ImpactSound || !ImpactEffect) { return; }
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	SpawnVfxAndSound();
	if (LoopSoundAudioComponent) { LoopSoundAudioComponent.Get()->Stop(); }

	if (HasAuthority())
	{
		UAbilitySystemComponent* ActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
		// TODO 应用GE
		if (!DamageEffectSpecHandle.IsValid() || !ActorAsc) { UE_LOG(ABaseProjectileLog, Warning, TEXT("无法应用GE!")); }
		else
		{
			// 应用GE到自身,这里的自身是OtherActor
			ActorAsc->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data);
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

void ABaseProjectile::Destroyed()
{
	if (!BIsHit && !HasAuthority()) { SpawnVfxAndSound(); }

	if (LoopSoundAudioComponent) { LoopSoundAudioComponent.Get()->Stop(); }
	Super::Destroyed();
}
