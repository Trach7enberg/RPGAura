// Copyright GGBAO 


#include "Characters/EnemyGhoul.h"

#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyGhoul::AEnemyGhoul()
{
	MaxWalkingSpeed = 125.0;
	AttackSocketName_LeftHand = "LeftHandAttackSocket";
	AttackSocketName_RightHand = "RightHandAttackSocket";
	EnemyHealthBar->SetRelativeLocation(FVector(0.0f, 0.0f, 74.0f));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -106.0f));
	GetCapsuleComponent()->SetCapsuleHalfHeight(105.0f);
	GetCapsuleComponent()->SetCapsuleRadius(73.646233f);
	GetCharacterMovement()->RotationRate = FRotator(0, 0, 150);

	WeaponLogicBaseComponent->SetBDoesNeedWeapon(false) ;
}
