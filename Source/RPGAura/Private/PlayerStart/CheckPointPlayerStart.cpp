// Copyright GGBAO 


#include "PlayerStart/CheckPointPlayerStart.h"

#include "Components/BoxComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameModes/RPGAuraGameModeBase.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMaterialLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(ACheckPointPlayerStartLog, All, All);

ACheckPointPlayerStart::ACheckPointPlayerStart(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	BoxCollision        = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	AutoMovePosition         = CreateDefaultSubobject<USceneComponent>("AutoMovePos");

	StaticMeshComponent->SetupAttachment(GetRootComponent());
	BoxCollision->SetupAttachment(GetRootComponent());
	AutoMovePosition->SetupAttachment(GetRootComponent());

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

bool ACheckPointPlayerStart::ShouldLoadTransform()
{
	// 检查点是固定的因此不需要更新变换
	return false;
}

void ACheckPointPlayerStart::LoadActorSaveInfo()
{
	// 从磁盘加载时,是否保持发光(如果序列化到磁盘时是发光的状态,那么加载时就会保持发光)?
	if (Reached)
	{
		BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		HandelGlowMaterial();
		CheckPointReached();
	}
}

void ACheckPointPlayerStart::HighLightActor()
{
	if (!StaticMeshComponent) { return; }
	StaticMeshComponent->SetRenderCustomDepth(true);
	StaticMeshComponent->SetCustomDepthStencilValue(CustomDepthStencilValue);
}

void ACheckPointPlayerStart::UnHighLightActor()
{
	if (!StaticMeshComponent) { return; }
	StaticMeshComponent->SetRenderCustomDepth(false);
	StaticMeshComponent->SetCustomDepthStencilValue(0);
}

void ACheckPointPlayerStart::GetMoveToLocation(FVector& OutDestination)
{
	OutDestination = AutoMovePosition->GetComponentLocation();
}

void ACheckPointPlayerStart::BeginPlay()
{
	Super::BeginPlay();
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ACheckPointPlayerStart::OnSphereOverlap);
	StaticMeshComponent->OnBeginCursorOver.AddDynamic(this, &ACheckPointPlayerStart::OnCursorBegin);
	StaticMeshComponent->OnEndCursorOver.AddDynamic(this, &ACheckPointPlayerStart::OnCursorEnd);
}

void ACheckPointPlayerStart::HandelGlowMaterial()
{
	if(LocalCheckPointDynamicMi){return;}
	LocalCheckPointDynamicMi = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, CheckPointMi);
	StaticMeshComponent->SetMaterial(0, LocalCheckPointDynamicMi);
}

void ACheckPointPlayerStart::OnSphereOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor*              OtherActor,
	UPrimitiveComponent* OtherComp,
	int32                OtherBodyIndex,
	bool                 bFromSweep,
	const FHitResult&    SweepResult)
{
	if (!OtherActor || !OtherActor->ActorHasTag(FRPGAuraGameplayTags::Get().Player) || !CheckPointMi) { return; }

	const auto PlayerIntF = Cast<IPlayerInterface>(OtherActor);
	if (!PlayerIntF) { return; }

	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandelGlowMaterial();
	CheckPointReached();

	if (!PlayerStartTag.IsValid() || PlayerStartTag.IsNone())
	{
		UE_LOG(ACheckPointPlayerStartLog, Error, TEXT("[%s]检查点无效!!"), TEXT(__FUNCTION__));
		return;
	}
	Reached = true;
	PlayerIntF->SaveProgress(PlayerStartTag);

	if (const auto MyGameMode = Cast<ARPGAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		
	{
		auto MapAssetName = GetWorld()->GetMapName();
		MapAssetName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
		MyGameMode->SaveWorldState(GetWorld(),MapAssetName);
	}
	else
	{
		UE_LOG(ACheckPointPlayerStartLog,
		       Error,
		       TEXT("[%s]检查点 %s:无法保存世界状态!!"),
		       TEXT(__FUNCTION__),
		       *PlayerStartTag.ToString());
	}
}

void ACheckPointPlayerStart::OnCursorBegin(UPrimitiveComponent* TouchedComponent) { HighLightActor(); }
void ACheckPointPlayerStart::OnCursorEnd(UPrimitiveComponent* TouchedComponent) { UnHighLightActor(); }
