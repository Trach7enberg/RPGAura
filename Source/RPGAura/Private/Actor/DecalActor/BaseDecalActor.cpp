// Copyright GGBAO 


#include "Actor/DecalActor/BaseDecalActor.h"

#include "Components/DecalComponent.h"

// Sets default values
ABaseDecalActor::ABaseDecalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	DecalComponent = CreateDefaultSubobject<UDecalComponent>("DecalComponent");
	SetRootComponent(DecalComponent);
}

// Called when the game starts or when spawned
void ABaseDecalActor::BeginPlay()
{
	Super::BeginPlay();
	SetDecalActorVisibility(false);
}

// Called every frame
void ABaseDecalActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ABaseDecalActor::SetDecalMaterial(UMaterialInterface* DecalMaterial) const
{
	if (!DecalComponent) { return; }
	DecalComponent->SetMaterial(0, DecalMaterial);
}
