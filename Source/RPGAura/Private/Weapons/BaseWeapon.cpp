// Copyright GGBAO 


#include "Weapons/BaseWeapon.h"

ABaseWeapon::ABaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

}

void ABaseWeapon::SetWeaponMeshCollision(bool Enabled) const
{
	WeaponMesh->SetCollisionEnabled(Enabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ABaseWeapon::BeginPlay() { Super::BeginPlay(); }
