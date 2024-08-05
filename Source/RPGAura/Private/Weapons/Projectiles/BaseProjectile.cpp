// Copyright GGBAO 


#include "Weapons/Projectiles/BaseProjectile.h"

#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
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
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
    Super::BeginPlay();
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnSphereOverlap);
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //
}