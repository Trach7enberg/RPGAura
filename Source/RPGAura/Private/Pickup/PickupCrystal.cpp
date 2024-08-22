// Copyright GGBAO 


#include "Pickup/PickupCrystal.h"

#include "Components/SphereComponent.h"

APickupCrystal::APickupCrystal()
{
	StaticMeshComponent->SetRelativeScale3D(FVector(0.4f));
	SphereComponent->SetRelativeLocation(FVector(0,-22.499999,5.833333));
	SphereComponent->InitSphereRadius(140.0f);
}
