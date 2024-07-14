// Copyright GGBAO 


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerStates/BasePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(AAuraCharacterLog, All, All);

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
	SpringArmComponent->bInheritPitch = false;
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

void AAuraCharacter::BeginPlay() { Super::BeginPlay(); }

UAbilitySystemComponent *AAuraCharacter::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

void AAuraCharacter::InitAbilityActorInfo()
{
	const auto MyPlayerState = GetPlayerState<ABasePlayerState>();
	if (!MyPlayerState)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("%s:MyPlayerState Cant be null"), *GetName());
		return;
	}

	AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
	AttributeSet = MyPlayerState->GetAttributeSet();
	AbilitySystemComponent->InitAbilityActorInfo(MyPlayerState, this);


}

void AAuraCharacter::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
	// 为服务器初始化能力角色信息
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// 为客户端初始化能力角色信息
	InitAbilityActorInfo();

}
