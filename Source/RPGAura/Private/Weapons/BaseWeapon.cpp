// Copyright GGBAO 


#include "Weapons/BaseWeapon.h"

#include "Interfaces/HighLightInterface.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(ABaseWeaponLog, All, All);

ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

	// 武器启用网络复制, 
	bReplicates = true;
	SetReplicatingMovement(true);
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	
}

void ABaseWeapon::Destroyed()
{
	if(WeaponMesh)
	{
		WeaponMesh->DestroyComponent();
	}
	Super::Destroyed();
}

bool ABaseWeapon::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}

void ABaseWeapon::OnRep_WeaponMesh()
{
	
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

FVector ABaseWeapon::GetWeaponSocketLocByName(const FName& SocketName) const
{
	if (!WeaponMesh) { return FVector::Zero(); }
	return WeaponMesh->GetSocketLocation(SocketName);
}

void ABaseWeapon::SetWeaponPhysics(const bool Enable) const
{
	if (!WeaponMesh) { return; }

	WeaponMesh->SetSimulatePhysics(Enable);
	WeaponMesh->SetEnableGravity(Enable);
	WeaponMesh->SetCollisionEnabled((Enable)
		                                ? ECollisionEnabled::Type::PhysicsOnly
		                                : ECollisionEnabled::Type::NoCollision);
}

void ABaseWeapon::SetWeaponMaterial(const int I, UMaterialInstance* MaterialInstance) const
{
	if (!WeaponMesh) { return; }
	WeaponMesh->SetMaterial(I, MaterialInstance);
}

void ABaseWeapon::SetCollisionEnabled(const ECollisionEnabled::Type NewType) const
{
	if (!WeaponMesh) { return; }
	WeaponMesh->SetCollisionEnabled(NewType);
}

UAnimInstance* ABaseWeapon::GetWeaponAnimInstance() const
{
	if (!WeaponMesh) { return nullptr; }
	return WeaponMesh->GetAnimInstance();
}

void ABaseWeapon::AddImpulse(const FVector& Impulse, const FName BoneName, const bool bVelChange) const
{
	if(WeaponMesh)
	{
		WeaponMesh->AddImpulse(Impulse, BoneName, bVelChange);
	}
}

USkeletalMeshComponent* ABaseWeapon::GetWeaponMesh() const
{
	return WeaponMesh.Get();
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseWeapon, WeaponMesh);
}
