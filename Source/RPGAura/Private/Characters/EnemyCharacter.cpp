// Copyright GGBAO 


#include "Characters/EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "RPGAura/RPGAura.h"

DEFINE_LOG_CATEGORY_STATIC(AEnemyCharacterLog, All, All);

AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");

	// 多人联机设置
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
	// AutoPossessAI = EAutoPossessAI::Disabled;
}


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(AbilitySystemComponent);

	InitAbilityActorInfo();

	// 要把胶囊体的visibility通道设置为Block,否则无法触发MouseOver等事件
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	OnBeginCursorOver.AddDynamic(this, &AEnemyCharacter::OnMouseOver);
	OnEndCursorOver.AddDynamic(this, &AEnemyCharacter::EndMouseOver);

}

void AEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(AbilitySystemComponent);
	if (!MyAsc)
	{
		UE_LOG(AEnemyCharacterLog, Error, TEXT("MyAsc Cant be null"));
		return;
	}
	MyAsc->InitSetting();
}


void AEnemyCharacter::HighLightActor()
{
	if (!GetMesh() || !WeaponLogicBaseComponent)
	{
		return;
	}
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	WeaponLogicBaseComponent->HighLight();
}

void AEnemyCharacter::UnHighLightActor()
{
	if (!GetMesh() || !WeaponLogicBaseComponent)
	{
		return;
	}
	GetMesh()->SetRenderCustomDepth(false);
	GetMesh()->SetCustomDepthStencilValue(0);
	WeaponLogicBaseComponent->UnHighLight();
}

void AEnemyCharacter::OnMouseOver(AActor *TouchedActor)
{
	Super::HighLight();
	UE_LOG(AEnemyCharacterLog, Warning, TEXT("%s,HightLight"), *TouchedActor->GetName());

}

void AEnemyCharacter::EndMouseOver(AActor *TouchedActor)
{
	Super::UnHighLight();
	UE_LOG(AEnemyCharacterLog, Warning, TEXT("%s,UnHightLight"), *TouchedActor->GetName());
}
