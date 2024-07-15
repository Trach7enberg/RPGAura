// Copyright GGBAO 


#include "Dev/EffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Characters/AuraCharacter.h"
#include "Components/SphereComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "UI/HUD/BaseHUD.h"

DEFINE_LOG_CATEGORY_STATIC(AEffectActorLog, All, All);

AEffectActor::AEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(250.0f);
	SphereComponent->SetHiddenInGame(false);

}

void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnBeginOverBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AEffectActor::OnEndOverBegin);
}

void AEffectActor::OnBeginOverBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                    UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult &SweepResult)
{
	const auto Character = Cast<AAuraCharacter>(OtherActor);
	if (!Character) { return; }

	const auto Controller = Cast<APlayerController>(Character->GetController());
	if (!Controller) { return; }

	const auto HUD = Cast<ABaseHUD>(Controller->GetHUD());
	if (!HUD) { return; }

	const auto AbilitySystem = Cast<IAbilitySystemInterface>(OtherActor);
	if (!AbilitySystem) { return; }

	const auto AbilitySystemComponent = AbilitySystem->GetAbilitySystemComponent();
	if (!AbilitySystemComponent) { return; }


	if (!AbilitySystemComponent->GetAttributeSet(UBaseAttributeSet::StaticClass())) { return; }

	const auto MyAttributeSet = Cast<UBaseAttributeSet>(
		AbilitySystemComponent->GetAttributeSet(UBaseAttributeSet::StaticClass()));
	if (!MyAttributeSet) { return; }


	// TODO 暂时使用const_cast来强制更改,以后用GamePlay Effects 来修改
	auto MutableAs = const_cast<UBaseAttributeSet *>(MyAttributeSet);

	if (!MutableAs) { return; }
	MutableAs->SetCurrentHealth(MyAttributeSet->GetCurrentHealth() + (-10.0f));
	MutableAs->SetCurrentMana(MyAttributeSet->GetCurrentMana() + (-10.0f));

	UE_LOG(AEffectActorLog, Warning, TEXT("Effect"));


	// Destroy();

}

void AEffectActor::OnEndOverBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor,
                                  UPrimitiveComponent *OtherComp, int32 OtherBodyIndex) {}
