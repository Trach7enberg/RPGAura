// Copyright GGBAO 


#include "Pickup/PickupPotion.h"

#include "Components/SphereComponent.h"

APickupPotion::APickupPotion()
{
	SphereComponent->InitSphereRadius(140.0f);
	StaticMeshComponent->SetRelativeScale3D(FVector(0.3f));
}
