// Copyright GGBAO 


#include "Characters/EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "RPGAura/RPGAura.h"
DEFINE_LOG_CATEGORY_STATIC(AEnemyCharacterLog, All, All);


void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 要把胶囊体的visibility通道设置为Block,否则无法触发MouseOver等事件
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);


	OnBeginCursorOver.AddDynamic(this, &AEnemyCharacter::OnMouseOver);
	OnEndCursorOver.AddDynamic(this, &AEnemyCharacter::EndMouseOver);
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
