// Copyright GGBAO 


#include "Characters/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameInstance/BaseGameInstance.h"
#include "GameModes/RPGAuraGameModeBase.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "PlayerStates/BasePlayerState.h"
#include "Save/LoadScreen/LoadScreenSave.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "UI/HUD/BaseHUD.h"

DEFINE_LOG_CATEGORY_STATIC(AAuraCharacterLog, All, All);

AAuraCharacter::AAuraCharacter()
{
	Tags.Add(FRPGAuraGameplayTags::Get().Player);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraComponent    = CreateDefaultSubobject<UCameraComponent>("Camera");


	SpringArmComponent->SetupAttachment(GetRootComponent());
	CameraComponent->SetupAttachment(SpringArmComponent);


	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 85));
	SpringArmComponent->SetRelativeRotation(FRotator(-45, 0, 0));
	SpringArmComponent->TargetArmLength  = 750.f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->bInheritPitch    = false;
	SpringArmComponent->bInheritRoll     = false;
	SpringArmComponent->bInheritYaw      = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate              = FRotator(0, 400, 0);

	// 限制角色的移动能力在平面上 
	GetCharacterMovement()->bConstrainToPlane   = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationRoll  = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;

	if (GetMesh()) { GetMesh()->SetRelativeLocation(FVector(0, 0, -85)); }

	// Aura角色职业默认为法师
	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::BeginPlay() { Super::BeginPlay(); }

UAbilitySystemComponent* AAuraCharacter::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

void AAuraCharacter::InitAbilityActorInfo()
{
	Super::InitAbilityActorInfo();
	const auto MyPlayerState = GetPlayerState<ABasePlayerState>();
	if (!MyPlayerState || !GetMyGameMode())
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("%s:MyPlayerState 不能为null"), *GetName());
		return;
	}

	AbilitySystemComponent = MyPlayerState->GetAbilitySystemComponent();
	AttributeSet           = MyPlayerState->GetAttributeSet();

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if (!MyAsc)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("MyAsc 不能为null"));
		return;
	}
	MyAsc->InitAbilityActorInfo(MyPlayerState, this);
	MyAsc->InitSetting();


	LoadProgress();
	GetMyGameMode()->LoadWorldState(GetWorld());

	RegisterGameplayTagEvent();
}

void AAuraCharacter::MulticastHandleDeath(const FVector& Impulse, const bool IsFinalBlow)
{
	if (!GetWorld() || !GetMyGameMode())
	{
		UE_LOG(AAuraCharacterLog,Error,TEXT("[%s]无法获取游戏模式!"),TEXT(__FUNCTION__));
		return;
	}
	
	FTimerDelegate TimerDelegate{};
	TimerDelegate.BindLambda([this]()
	{
		GetMyGameMode()->CharacterDie(this);
	});

	GetWorldTimerManager().SetTimer(DeathTimer,TimerDelegate,DeathTime,false);
	Super::MulticastHandleDeath(Impulse,IsFinalBlow);
	CameraComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AAuraCharacter::InitHUD()
{
	const auto Pc = Cast<APlayerController>(GetController());
	if (!Pc)
	{
		UE_LOG(AAuraCharacterLog,
		       Error,
		       TEXT("[模式:%s]:[%s]的控制器无效! (IsLocalPalyerController?:%s)"),
		       *GetNetModeStr(),
		       *GetName(),
		       IsLocallyControlled() ? TEXT("是") : TEXT("否"));
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

void AAuraCharacter::SetCastShockAnimState(const bool Enabled) { bIsCastShockAnimLoop = Enabled; }

bool AAuraCharacter::GetCastShockAnimState() { return bIsCastShockAnimLoop; }

void AAuraCharacter::AddToPlayerXP(const int32 AddedXp)
{
	if (!GetPlayerState()) { return; }
	const auto Ps = Cast<ABasePlayerState>(GetPlayerState());
	if (!Ps) { return; }

	Ps->AddToPlayerXP(AddedXp);
}

int32 AAuraCharacter::GetPlayerCurrentXP()
{
	if (!GetMyPlayerState()) { return 0; }
	return GetMyPlayerState()->GetPlayerCurrentXP();
}

bool AAuraCharacter::CanBeLevelUp()
{
	if (!GetMyPlayerState() || !GetMyGiSubSystem()) { return false; }

	const auto NextLevelMinimumXpRequired = GetMyPlayerState()->GetMinimumXpRequiredForLevel(GetCharacterLevel() + 1);

	return (NextLevelMinimumXpRequired != 0 && GetPlayerCurrentXP() >= NextLevelMinimumXpRequired);
}

void AAuraCharacter::LevelUp()
{
	if (!GetMyPlayerState() || !GetMyGiSubSystem() || !GetAbilitySystemComponent()) { return; }
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!MyAsc) { return; }

	const auto NextLevel = GetMyGiSubSystem()->GetLevelCorrespondingToXP(
	                                                                     GetCharacterClass(),
	                                                                     GetPlayerCurrentXP(),
	                                                                     GetCharacterLevel());

	const auto TempCurrentLevel = GetCharacterLevel();
	if (NextLevel == GetCharacterLevel()) { return; }

	GetMyPlayerState()->SetPlayerLevel(NextLevel);
	// 先前的等级与当前等级之差
	const auto LevelDifference = GetCharacterLevel() - TempCurrentLevel;
	AddToAttributesPoints(GetMyPlayerState()->GetAttributePointsReward(GetCharacterLevel(), LevelDifference));
	AddToSpellPoints(GetMyPlayerState()->GetSpellPointsReward(GetCharacterLevel(), LevelDifference));
	MyAsc->UpdateAbilityStatusWhenLevelUp(GetCharacterLevel());

	// 升级特效的位置是世界位置
	MulticastVfx(FRPGAuraGameplayTags::Get().Abilities_Vfx_Misc_LevelUp,
	             LevelUpEffect,
	             FTransform{FRotator(90, 0, 180), GetActorLocation()},
	             EAttachLocation::Type::KeepWorldPosition);
}

int32 AAuraCharacter::GetAttributePointsReward(const int32 InCharacterLevel)
{
	if (!GetMyPlayerState()) { return 0; }
	return GetMyPlayerState()->GetAttributePointsReward(InCharacterLevel);
}

int32 AAuraCharacter::GetSpellPointsReward(const int32 InCharacterLevel)
{
	if (!GetMyPlayerState()) { return 0; }
	return GetMyPlayerState()->GetSpellPointsReward(InCharacterLevel, 1);
}

void AAuraCharacter::AddToSpellPoints(const int32 Points)
{
	if (!GetMyPlayerState()) { return; }
	GetMyPlayerState()->AddToAssignableSpellPoints(Points);
}

void AAuraCharacter::AddToAttributesPoints(const int32 Points)
{
	if (!GetMyPlayerState()) { return; }
	GetMyPlayerState()->AddToAssignableAttributePoints(Points);
}

int32 AAuraCharacter::GetCurrentAssignableAttributePoints()
{
	if (!GetMyPlayerState()) { return 0; }
	return GetMyPlayerState()->GetAssignableAttributePoints();
}

int32 AAuraCharacter::GetCurrentAssignableSpellPoints()
{
	if (!GetMyPlayerState()) { return 0; }
	return GetMyPlayerState()->GetAssignableSpellPoints();
}

void AAuraCharacter::SaveProgress(const FName& CheckPointTag)
{
	if (!GetMyGameMode() || !GetMyGi() || !GetAbilitySystemComponent() || !GetAttributeSet()) { return; }

	auto       SaveGamObj = GetMyGameMode()->GetCurrentInGameSaveData();
	const auto MyAcs      = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!SaveGamObj || !MyAcs) { return; }

	// 同时也设置Gi里的PlayerStartTag,以便选择出生点时有正确的重生点
	GetMyGi()->PlayerStartTag = CheckPointTag;
	// 持久化角色相关信息
	SaveGamObj->BisFirstLoad              = false;
	SaveGamObj->PlayerStartTag            = CheckPointTag;
	SaveGamObj->PlayerLevel               = GetCharacterLevel();
	SaveGamObj->PlayerXP                  = GetPlayerCurrentXP();
	SaveGamObj->AssignableAttributePoints = GetCurrentAssignableAttributePoints();
	SaveGamObj->AssignableSpellPoints     = GetCurrentAssignableSpellPoints();
	// 持久化角色属性
	SaveGamObj->Vigor        = UBaseAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
	SaveGamObj->Strength     = UBaseAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
	SaveGamObj->Resilience   = UBaseAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
	SaveGamObj->Intelligence = UBaseAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());

	MyAcs->GetAbilityToSave(SaveGamObj->SavedAbilities);

	GetMyGameMode()->SaveLoadSlotData(SaveGamObj);
}

void AAuraCharacter::LoadProgress()
{
	if (!GetMyGameMode() || !GetMyGi() || !GetMyGiSubSystem() || !GetAbilitySystemComponent() || !GetAttributeSet())
	{
		return;
	}
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
	const auto LoadData = GetMyGameMode()->GetCurrentInGameSaveData();
	if (!LoadData || !MyAsc)
	{
		UE_LOG(AAuraCharacterLog, Error, TEXT("[%s]无法加载角色存档进度,将会加载默认进度!"), TEXT(__FUNCTION__));
		InitAllAttributes(true);
		AddCharacterAbilities();
		return;
	}
	const auto Ps = GetMyPlayerState();

	// 不是第一次加载的话, 从磁盘加载角色属性
	if (!LoadData->BisFirstLoad)
	{
		// 从磁盘加载角色的等级相关信息
		Ps->SetPlayerLevel(LoadData->PlayerLevel, false);
		Ps->SetPlayerXP(LoadData->PlayerXP, false);
		Ps->SetAssignableAttributePoints(LoadData->AssignableAttributePoints);
		Ps->SetAssignableSpellPoints(LoadData->AssignableSpellPoints);

		// 从磁盘加载角色的属性(应用GE)
		GetMyGiSubSystem()->InitializeDefaultAttributes(GetAbilitySystemComponent(),
		                                                CharacterClass,
		                                                GetCharacterLevel(),
		                                                FRPGAuraGameplayTags::PrimaryGameplayTagsContainer,
		                                                TArray{
			                                                LoadData->Strength,
			                                                LoadData->Intelligence,
			                                                LoadData->Resilience,
			                                                LoadData->Vigor
		                                                });
		// 从磁盘加载能力
		MyAsc->AddAbilitiesFromLoadData(LoadData);
		MyAsc->AddListenPassiveAbilities(CharacterClass, GetCharacterLevel());
	}
	else
	{
		InitAllAttributes(true);
		AddCharacterAbilities();
	}
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
