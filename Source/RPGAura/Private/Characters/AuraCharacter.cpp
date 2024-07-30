// Copyright GGBAO 


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlayerStates/BasePlayerState.h"
#include "UI/HUD/BaseHUD.h"

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

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(0, 0, -85));
	}
}

void AAuraCharacter::BeginPlay()
{
	Super::BeginPlay();
}

UAbilitySystemComponent *AAuraCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

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

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if (!MyAsc)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("MyAsc Cant be null"));
		return;
	}
	MyAsc->InitAbilityActorInfo(MyPlayerState, this);
	MyAsc->InitSetting();


}

void AAuraCharacter::InitHUD() const
{
	const auto Pc = Cast<APlayerController>(GetController());
	if (!Pc)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("控制器无效!"));
		return;
	}
	const auto Hud = Cast<ABaseHUD>(Pc->GetHUD());
	if (!Hud)
	{
		return;
	}
	Hud->InitHudMainWidget();
}

int32 AAuraCharacter::GetCharacterLevel()
{
	const auto MyPlayerState = GetPlayerState<ABasePlayerState>();
	if (!MyPlayerState)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("%s:PlayerState Cant be null"), *GetName());
		return 0;
	}

	return MyPlayerState->GetPlayerLevel();
}

void AAuraCharacter::PossessedBy(AController *NewController)
{
	Super::PossessedBy(NewController);
	// 为服务器初始化能力角色信息
	InitAbilityActorInfo();

	

	InitAttributes(DefaultPrimaryAttributesGameplayEffect);
	InitAttributes(DefaultSecondaryPrimaryAttributesGameplayEffect);
	InitAttributes(DefaultVitalAttributesGameplayEffect);
	InitHUD();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// 为客户端初始化能力角色信息
	InitAbilityActorInfo();

	InitAttributes(DefaultPrimaryAttributesGameplayEffect);
	InitAttributes(DefaultSecondaryPrimaryAttributesGameplayEffect);
	InitAttributes(DefaultVitalAttributesGameplayEffect);
	InitHUD();
}
