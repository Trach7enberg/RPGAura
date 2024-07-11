// Copyright GGBAO 


#include "Components/WeaponLogicBaseComponent.h"

#include "Characters/CharacterBase.h"
#include "Weapons/BaseWeapon.h"


DEFINE_LOG_CATEGORY_STATIC(MyWeaponLogicBaseComponentLog, All, All);

UWeaponLogicBaseComponent::UWeaponLogicBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	

}

void UWeaponLogicBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner() || !WeaponBp)
	{
		UE_LOG(MyWeaponLogicBaseComponentLog, Error, TEXT("Owner Error"));
		return;
	}

	AttachWeaponToSocket(Cast<ACharacterBase>(GetOwner()), WeaponAttachSocketName);
}

void UWeaponLogicBaseComponent::AttachWeaponToSocket(ACharacterBase *Character, FName &SocketName)
{
	const auto Mesh = Character->GetMesh();
	if (!Character || !Mesh) { return; }

	CurrentWeapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor(WeaponBp));

	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                 WeaponAttachSocketName);
		CurrentWeapon->SetOwner(Character);
		CurrentWeapon->SetWeaponMeshCollision(false);
	}
}
