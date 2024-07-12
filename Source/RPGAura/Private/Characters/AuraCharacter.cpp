// Copyright GGBAO 


#include "Characters/AuraCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


AAuraCharacter::AAuraCharacter()
{

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");

	SpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 85));
	SpringArmComponent->SetRelativeRotation(FRotator(-45, 0, 0));
	SpringArmComponent->TargetArmLength = 750.f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bInheritPitch=false;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bInheritYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400, 0);

	// 限制角色的移动能力在平面上 
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	
	if (GetMesh()) { GetMesh()->SetRelativeLocation(FVector(0, 0, -85)); }
}
