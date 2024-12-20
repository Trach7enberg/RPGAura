// Copyright GGBAO 


#include "Weapons/Projectiles/BaseProjectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "RPGAura/RPGAura.h"

DEFINE_LOG_CATEGORY_STATIC(ABaseProjectileLog, All, All);

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	bIgnoreFriend = true;
	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComponent);
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(SphereComponent);
	
	
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SphereComponent->SetCollisionProfileName(COLLISION_PRESET_PROJECTILE);
	
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;

	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}


void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	// 让射弹的运动可以被复制
	SetReplicateMovement(true);
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
	
	
}

void ABaseProjectile::EnableHomingActorOnDestroyedEvent()
{
	if(!GetProjectileMovementComponent() || !GetProjectileMovementComponent()->HomingTargetComponent.Get()){return;}

	const auto HomingCompOwner = GetProjectileMovementComponent()->HomingTargetComponent.Get()->GetOwner();
	const auto CombatInt = Cast<ICombatInterface>(HomingCompOwner);
	if(!CombatInt){return;}

	if(CombatInt->GetPreOnDeathDelegate().IsAlreadyBound(this,&ABaseProjectile::HomingActorOnPreDestroyed)){return;}

	CombatInt->GetPreOnDeathDelegate().AddDynamic(this,&ABaseProjectile::HomingActorOnPreDestroyed);
}

void ABaseProjectile::HomingActorOnPreDestroyed(AActor* Actor)
{
	if(!GetProjectileMovementComponent()){return;}
	GetProjectileMovementComponent()->bIsHomingProjectile = false;
}

void ABaseProjectile::Destroyed()
{
	Super::Destroyed();
}

void ABaseProjectile::LifeSpanExpired() { Super::LifeSpanExpired(); }
