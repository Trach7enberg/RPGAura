// Copyright GGBAO 


#include "Characters/CharacterBase.h"

#include "Components/WeaponLogicBaseComponent.h"
#include "Interfaces/HighLightInterface.h"

// Sets default values
ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponLogicBaseComponent = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");


	if (GetMesh()) { GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); }
}


void ACharacterBase::BeginPlay() { Super::BeginPlay(); }

bool ACharacterBase::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}

void ACharacterBase::HighLight()
{
	if (!CanHighLight()) { return; }
	Cast<IHighLightInterface>(this)->HighLightActor();
}

void ACharacterBase::UnHighLight()
{
	if (!CanHighLight()) { return; }
	Cast<IHighLightInterface>(this)->UnHighLightActor();
}

UAbilitySystemComponent *ACharacterBase::GetAbilitySystemComponent() const { return AbilitySystemComponent; }
