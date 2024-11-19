// Copyright GGBAO 


#include "PlayerStart/CheckPointBeacon.h"

#include "Components/BoxComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"

void ACheckPointBeacon::HighLightActor()
{
	if(!Reached)
	{
		Super::HighLightActor();
	}
}
void ACheckPointBeacon::UnHighLightActor()
{
	Super::UnHighLightActor();
}

void ACheckPointBeacon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	
	if (!OtherActor || !OtherActor->ActorHasTag(FRPGAuraGameplayTags::Get().Player)) { return; }
	
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HandelGlowMaterial();
	CheckPointReached();
	Reached = true;
}
