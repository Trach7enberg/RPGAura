// Copyright GGBAO 


#include "Components/WeaponLogicBaseComponent.h"

#include "Characters/CharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/BaseWeapon.h"


DEFINE_LOG_CATEGORY_STATIC(MyWeaponLogicBaseComponentLog, All, All);

UWeaponLogicBaseComponent::UWeaponLogicBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	BShouldDestroyWeapon = true;
	BDoesNeedWeapon = true;
	WeaponBp = nullptr;

	// 组件启用网络复制
	SetIsReplicatedByDefault(true);	
}


void UWeaponLogicBaseComponent::SetWeaponCollisionEnabled(ECollisionEnabled::Type NewType) const
{
	if (!CurrentWeapon) { return; }

	CurrentWeapon->SetCollisionEnabled(NewType);
}

UAnimInstance* UWeaponLogicBaseComponent::GetCurrentWeaponAnimInstanceClass() const
{
	if (!CurrentWeapon) { return nullptr; }
	return CurrentWeapon->GetWeaponAnimInstance();
}

void UWeaponLogicBaseComponent::AddWeaponImpulse(const FVector& Impulse, const FName BoneName,
                                                 const bool bVelChange) const
{
	if (CurrentWeapon) { CurrentWeapon->AddImpulse(Impulse, BoneName, bVelChange); }
}

USkeletalMeshComponent* UWeaponLogicBaseComponent::GetWeaponMesh() const
{
	if (!CurrentWeapon) { return nullptr; }
	return CurrentWeapon->GetWeaponMesh();
}

void UWeaponLogicBaseComponent::BeginPlay() { Super::BeginPlay(); }

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
	if (!CurrentWeapon || !BDoesNeedWeapon) { return FVector::Zero(); }
	return CurrentWeapon->GetWeaponSocketLocByName(SocketName);
}

void UWeaponLogicBaseComponent::DetachWeapon() const
{
	if (!CurrentWeapon || !BDoesNeedWeapon) { return; }
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
}

void UWeaponLogicBaseComponent::SetWeaponPhysics(bool Enable) const
{
	if (!CurrentWeapon || !BDoesNeedWeapon) { return; }
	CurrentWeapon->SetWeaponPhysics(true);
}

void UWeaponLogicBaseComponent::SetWeaponMaterial(const int I, UMaterialInstance* MaterialInstance) const
{
	if (!CurrentWeapon || !BDoesNeedWeapon) { return; }

	CurrentWeapon->SetWeaponMaterial(I, MaterialInstance);
}


void UWeaponLogicBaseComponent::AttachWeaponToSocket(ACharacterBase* Owner, FName SocketName)
{
	const auto Mesh = Owner->GetMesh();
	
	if (!Mesh || !BDoesNeedWeapon || !WeaponBp) { return; }

	if (!Owner)
	{
		UE_LOG(MyWeaponLogicBaseComponentLog, Error, TEXT("Owner Error"));
		return;
	}

	if(!Owner->HasAuthority()){return;}
	
	CurrentWeapon = Cast<ABaseWeapon>(GetWorld()->SpawnActor(WeaponBp));
	
	if(!CurrentWeapon){return;}

	CurrentWeapon->AttachToComponent(Mesh,
									 FAttachmentTransformRules::SnapToTargetIncludingScale,
									 WeaponAttachSocketName);
	CurrentWeapon->SetOwner(Owner);
	CurrentWeapon->SetWeaponMeshCollision(false);
}

void UWeaponLogicBaseComponent::DestroyComponent(bool bPromoteChildren)
{
	if (CurrentWeapon && BShouldDestroyWeapon && BDoesNeedWeapon) { CurrentWeapon->Destroy(); }
	Super::DestroyComponent(bPromoteChildren);
}


void UWeaponLogicBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponLogicBaseComponent, CurrentWeapon);
}

void UWeaponLogicBaseComponent::OnRep_CurrentWeapon(ABaseWeapon* OldWeapon)
{
	
}
