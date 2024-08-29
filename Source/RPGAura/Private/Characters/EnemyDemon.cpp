// Copyright GGBAO 


#include "Characters/EnemyDemon.h"

#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyDemon::AEnemyDemon()
{
	AttackSocketName_LeftHand = "LeftHandAttackSocket";
	AttackSocketName_RightHand = "RightHandAttackSocket";
	EnemyHealthBar->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -106.0f));
	GetCapsuleComponent()->SetCapsuleHalfHeight(50.0f);
	GetCapsuleComponent()->SetCapsuleRadius(30.0f);
	GetCharacterMovement()->RotationRate = FRotator(0, 0, 150);

	WeaponLogicBaseComponent->SetBDoesNeedWeapon(false) ;
}
