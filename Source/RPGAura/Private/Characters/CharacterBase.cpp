// Copyright GGBAO 


#include "Characters/CharacterBase.h"

#include "Components/WeaponLogicBaseComponent.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponLogicBaseComponent = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");

	if (GetMesh()) { GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); }
}

void ACharacterBase::BeginPlay() { Super::BeginPlay(); }

void ACharacterBase::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
