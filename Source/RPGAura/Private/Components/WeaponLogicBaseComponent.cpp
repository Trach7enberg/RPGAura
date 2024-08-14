// Copyright GGBAO 


#include "Components/WeaponLogicBaseComponent.h"

#include "Characters/CharacterBase.h"
#include "Weapons/BaseWeapon.h"


DEFINE_LOG_CATEGORY_STATIC(MyWeaponLogicBaseComponentLog, All, All);

UWeaponLogicBaseComponent::UWeaponLogicBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	BShouldDestroyWeapon = true;
}


void UWeaponLogicBaseComponent::SetWeaponCollisionEnabled(ECollisionEnabled::Type NewType) const
{
	if(!CurrentWeapon){return;}

	CurrentWeapon->SetCollisionEnabled(NewType);
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

void UWeaponLogicBaseComponent::HighLight() const
{
	if (!CurrentWeapon) { return; }
	CurrentWeapon->HighLight();
}

void UWeaponLogicBaseComponent::UnHighLight() const
{
	if (!CurrentWeapon) { return; }
	CurrentWeapon->UnHighLight();
}

FVector UWeaponLogicBaseComponent::GetWeaponSocketLocByName(const FName& SocketName) const
{
	if (!CurrentWeapon) { return FVector::Zero(); }
	return CurrentWeapon->GetWeaponSocketLocByName(SocketName);
}

void UWeaponLogicBaseComponent::DetachWeapon() const
{
	if (!CurrentWeapon) { return; }
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
}

void UWeaponLogicBaseComponent::SetWeaponPhysics(bool Enable) const
{
	if (!CurrentWeapon) { return; }
	CurrentWeapon->SetWeaponPhysics(true);
}

void UWeaponLogicBaseComponent::SetWeaponMaterial(const int I, UMaterialInstance* MaterialInstance) const
{
	if (!CurrentWeapon) { return; }

	CurrentWeapon->SetWeaponMaterial(I, MaterialInstance);
}


void UWeaponLogicBaseComponent::AttachWeaponToSocket(ACharacterBase* Character, FName& SocketName)
{
	const auto Mesh = Character->GetMesh();
	if (!Character || !Mesh) { return; }

	CurrentWeapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor(WeaponBp));

	if (CurrentWeapon)
	{
		CurrentWeapon->AttachToComponent(Mesh,
		                                 FAttachmentTransformRules::SnapToTargetIncludingScale,
		                                 WeaponAttachSocketName);
		CurrentWeapon->SetOwner(Character);
		CurrentWeapon->SetWeaponMeshCollision(false);
	}
}

void UWeaponLogicBaseComponent::DestroyComponent(bool bPromoteChildren)
{
	if (CurrentWeapon && BShouldDestroyWeapon) { CurrentWeapon->Destroy(); }
	Super::DestroyComponent(bPromoteChildren);
}
