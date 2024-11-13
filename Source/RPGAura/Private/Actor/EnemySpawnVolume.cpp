// Copyright GGBAO 


#include "Actor/EnemySpawnVolume.h"

#include "Actor/AuraEnemySpawnPoint.h"
#include "Characters/AuraCharacter.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(AEnemySpawnVolumeLog, All, All);

AEnemySpawnVolume::AEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;
	BoxVolume                     = CreateDefaultSubobject<UBoxComponent>("BoxVolume");
	SetRootComponent(BoxVolume);
	BoxVolume->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxVolume->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxVolume->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

bool AEnemySpawnVolume::ShouldLoadTransform() { return false; }
void AEnemySpawnVolume::GetMoveToLocation(FVector& OutDestination) {}

void AEnemySpawnVolume::LoadActorSaveInfo()
{
	// 磁盘加载状态时,如果该生成点已经触发过则销毁
	if (bReached) { Destroy(); }
}

void AEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	BoxVolume->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawnVolume::OnSphereOverlap);
}

void AEnemySpawnVolume::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
                                        AActor*              OtherActor,
                                        UPrimitiveComponent* OtherComp,
                                        int32                OtherBodyIndex,
                                        bool                 bFromSweep,
                                        const FHitResult&    SweepResult)
{
	const auto Player = Cast<AAuraCharacter>(OtherActor);
	if (!Player) { return; }


	BoxVolume->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	bReached = true;
	for (const auto& SpawnPoint : SpawnPoints)
	{
		if (!IsValid(SpawnPoint)) { continue; }
		SpawnPoint->SpawnEnemy();
	}
}
