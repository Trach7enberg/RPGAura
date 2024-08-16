// Copyright GGBAO 


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Camera/CameraComponent.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStates/BasePlayerState.h"
#include "UI/HUD/BaseHUD.h"

DEFINE_LOG_CATEGORY_STATIC(AAuraCharacterLog, All, All);

AAuraCharacter::AAuraCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");

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

	// Aura角色职业默认为法师
	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::BeginPlay() { Super::BeginPlay(); }

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

void AAuraCharacter::InitAbilityActorInfo()
{
	const auto MyPlayerState = GetPlayerState<ABasePlayerState>();
	if (!MyPlayerState)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("%s:MyPlayerState 不能为null"), *GetName());
		return;
	}

	AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
	AttributeSet = MyPlayerState->GetAttributeSet();

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if (!MyAsc)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("MyAsc 不能为null"));
		return;
	}
	MyAsc->InitAbilityActorInfo(MyPlayerState, this);
	MyAsc->InitSetting();
	InitAllAttributes(true);
	
	RegisterGameplayTagEvent();

	AddCharacterAbilities();
}


void AAuraCharacter::InitHUD() const
{
	const auto Pc = Cast<APlayerController>(GetController());
	if (!Pc)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("[模式:%s]:[%s]的控制器无效! (IsLocalPalyerController?:%s)"), *GetNetModeStr(),
		       *GetName(), IsLocallyControlled() ? TEXT("是") : TEXT("否"));
		return;
	}
	const auto Hud = Cast<ABaseHUD>(Pc->GetHUD());
	if (!Hud) { return; }
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
	CharacterLevel = MyPlayerState->GetPlayerLevel();
	return CharacterLevel;
}

void AAuraCharacter::UpdateCharacterFacingTarget(const FVector& TargetLoc)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, TargetLoc);
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// 为服务器初始化能力角色信息
	InitAbilityActorInfo();

	InitHUD();

	
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// 为客户端初始化能力角色信息
	InitAbilityActorInfo();

	InitHUD();
}

FString AAuraCharacter::GetNetModeStr() const
{
	FString NetMode = "Client";

	if (HasAuthority()) { NetMode = "Server"; }
	return NetMode;
}

void AAuraCharacter::OnGrantedTag_HitReact(const FGameplayTag Tag, int32 NewTagCount)
{
	// TODO 待实现 玩家的受击逻辑
}
