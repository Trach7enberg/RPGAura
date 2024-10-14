// Copyright GGBAO 


#include "Characters/EnemyGhoul.h"

#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyGhoul::AEnemyGhoul()
{
	ImpulseFactorMesh /= 2;
	MaxWalkingSpeed = 125.0;
	AttackSocketName_LeftHand = "LeftHandAttackSocket";
	AttackSocketName_RightHand = "RightHandAttackSocket";
	EnemyHealthBar->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetCapsuleComponent()->SetCapsuleHalfHeight(85.0f);
	GetCapsuleComponent()->SetCapsuleRadius(57.76897f);
	GetCharacterMovement()->RotationRate = FRotator(0, 150, 0);

	WeaponLogicBaseComponent->SetBDoesNeedWeapon(false) ;
}
