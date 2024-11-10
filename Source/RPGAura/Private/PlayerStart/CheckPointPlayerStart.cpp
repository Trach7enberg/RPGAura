// Copyright GGBAO 


#include "PlayerStart/CheckPointPlayerStart.h"

#include "Components/BoxComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "Interfaces/PlayerInterface.h"

DEFINE_LOG_CATEGORY_STATIC(ACheckPointPlayerStartLog, All, All);

ACheckPointPlayerStart::ACheckPointPlayerStart(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	BoxCollision        = CreateDefaultSubobject<UBoxComponent>("BoxCollision");

	StaticMeshComponent->SetupAttachment(GetRootComponent());
	BoxCollision->SetupAttachment(GetRootComponent());

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

void ACheckPointPlayerStart::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACheckPointPlayerStart::OnSphereOverlap);
}

void ACheckPointPlayerStart::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor*              OtherActor,
	UPrimitiveComponent* OtherComp,
	int32                OtherBodyIndex,
	bool                 bFromSweep,
	const FHitResult&    SweepResult)
{
	if (!OtherActor || !OtherActor->ActorHasTag(FRPGAuraGameplayTags::Get().Player)) { return; }

	const auto PlayerIntF = Cast<IPlayerInterface>(OtherActor);
	if (!PlayerIntF) { return; }

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CheckPointReached();

	if (!PlayerStartTag.IsValid() || PlayerStartTag.IsNone())
	{
		UE_LOG(ACheckPointPlayerStartLog, Error, TEXT("[%s]检查点无效!!"), TEXT(__FUNCTION__));
		return;
	}
	PlayerIntF->SaveProgress(PlayerStartTag);
}


void ACheckPointPlayerStart::HandleMeshMaterial()
{
	// BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
