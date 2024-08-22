// Copyright GGBAO 


#include "Characters/EnemyCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "Components/WidgetComponent.h"
#include "Controller/BaseAIController.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "RPGAura/RPGAura.h"
#include "UI/Widgets/BaseUserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(AEnemyCharacterLog, All, All);

AEnemyCharacter::AEnemyCharacter()
{
	// 敌人有敌人标签
	Tags.Add(FRPGAuraGameplayTags::Get().Enemy);

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");

	// 多人联机设置
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
	// AutoPossessAI = EAutoPossessAI::Disabled;

	EnemyHealthBar = CreateDefaultSubobject<UWidgetComponent>("EnemyHealthBar");
	EnemyHealthBar->SetupAttachment(RootComponent);

	MaxWalkingSpeed = 250.f;

	AIControllerClass = ABaseAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorld;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		// 设置AI转向的丝滑度
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		// 注意是YAW!! Z 对应 YAW
		GetCharacterMovement()->RotationRate = FRotator(0, 200.0f, 0);
	}
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(AbilitySystemComponent);
	check(GetCharacterMovement());

	CharacterLevel = GetCharacterLevel();


	InitAbilityActorInfo();

	// 要把胶囊体的visibility通道设置为Block,否则无法触发MouseOver等事件
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	OnBeginCursorOver.AddDynamic(this, &AEnemyCharacter::OnMouseOver);
	OnEndCursorOver.AddDynamic(this, &AEnemyCharacter::EndMouseOver);

	const auto MyWidget = Cast<UBaseUserWidget>(EnemyHealthBar->GetUserWidgetObject());
	if (!MyWidget) { UE_LOG(AEnemyCharacterLog, Error, TEXT("设置血条widg的控制器失败")); }

	// 设置敌人血条的小部件控制器为当前角色 TODO 应该为敌人血条widget单独创建一个控制器类
	MyWidget->SetWidgetController(this);

	if (!AttributeSet) { return; }
	const auto MyAs = Cast<UBaseAttributeSet>(AttributeSet);

	if (!MyAs) { return; }

	// 绑定属性集生命值属性变化时候的委托
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(MyAs->GetCurrentHealthAttribute()).
	                             AddLambda(
		                             [this](const FOnAttributeChangeData& Data)
		                             {
			                             OnCurrentHealthAttributeChanged.Broadcast(
				                             Data.NewValue, Data.NewValue > Data.OldValue);
		                             });
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(MyAs->GetMaxHealthAttribute()).
	                             AddLambda(
		                             [this](const FOnAttributeChangeData& Data)
		                             {
			                             OnCurrentHealthAttributeChanged.Broadcast(
				                             Data.NewValue, Data.NewValue > Data.OldValue);
		                             });

	BroadCastHealthBarInit();
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if (!MyAsc)
	{
		UE_LOG(AEnemyCharacterLog, Error, TEXT("MyAsc 不能为 null"));
		return;
	}

	MyAsc->InitSetting();

	InitAllAttributes();
	RegisterGameplayTagEvent();

	AddCharacterAbilities();
}


void AEnemyCharacter::HighLightActor()
{
	if (!GetMesh() || !WeaponLogicBaseComponent) { return; }
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponLogicBaseComponent->HighLight();
}

void AEnemyCharacter::UnHighLightActor()
{
	if (!GetMesh() || !WeaponLogicBaseComponent) { return; }
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
	WeaponLogicBaseComponent->UnHighLight();
}

void AEnemyCharacter::Die() { Super::Die(); }

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 只在服务器上设置AI控制器,可以检查这个CurrentAiController为null就是不在服务器
	if (!HasAuthority()) { return; }
	CurrentAiController = Cast<ABaseAIController>(NewController);

	if (CurrentAiController && BehaviorTree)
	{
		// 初始化黑板和运行行为树
		CurrentAiController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->GetBlackboardAsset());
		CurrentAiController->RunBehaviorTree(BehaviorTree);
		// 设置黑板键,当前角色是不是远程攻击角色
		CurrentAiController->GetBlackboardComponent()->SetValueAsBool("IsRangCharacter", CharacterClass == ECharacterClass::Warrior);
	}
}

void AEnemyCharacter::OnMouseOver(AActor* TouchedActor) { Super::HighLight(); }

void AEnemyCharacter::EndMouseOver(AActor* TouchedActor) { Super::UnHighLight(); }


void AEnemyCharacter::BroadCastHealthBarInit() const
{
	if (!AttributeSet)
	{
		UE_LOG(AEnemyCharacterLog, Error, TEXT("AttributeSet 不能为 null"));
		return;
	}

	const auto MyAs = Cast<UBaseAttributeSet>(AttributeSet);
	OnCurrentHealthAttributeChanged.Broadcast(
		MyAs->GetCurrentHealth(), false);
	OnMaxHealthAttributeChanged.Broadcast(
		MyAs->GetMaxHealth(), false);
}
