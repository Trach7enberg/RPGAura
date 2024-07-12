// Copyright GGBAO 


#include "Weapons/BaseWeapon.h"

#include "Interfaces/HighLightInterface.h"

ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

}

void ABaseWeapon::BeginPlay() { Super::BeginPlay(); }

bool ABaseWeapon::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}

void ABaseWeapon::SetWeaponMeshCollision(bool Enabled) const
{
	WeaponMesh->SetCollisionEnabled(Enabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

void ABaseWeapon::HighLight()
{
	if (!CanHighLight()) { return; }
	Cast<IHighLightInterface>(this)->HighLightActor();
}

void ABaseWeapon::UnHighLight()
{
	if (!CanHighLight()) { return; }
	Cast<IHighLightInterface>(this)->UnHighLightActor();
}
