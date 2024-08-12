// Copyright GGBAO 


#include "Characters/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "Interfaces/HighLightInterface.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(ACharacterBaseLog, All, All);

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponLogicBaseComponent = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");

	if (GetMesh()) { GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); }
}


FVector ACharacterBase::GetCombatSocketLocation()
{
	return WeaponLogicBaseComponent->GetWeaponSocketLocByName(WeaponLogicBaseComponent->GetWeaponTipSocketName());
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponLogicBaseComponent);
}


void ACharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) { return; }

	if (!GetAbilitySystemComponent()) { return; }

	const auto Asc = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!Asc) { return; }
	Asc->AddCharacterAbilities(StartUpAbilities);
}

void ACharacterBase::InitAttributes(const TSubclassOf<UGameplayEffect> AttributesGameplayEffect,
                                    const float Level) const
{
	if (!GetAbilitySystemComponent() || !AttributesGameplayEffect)
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("[%s]无法初始化属性，因为没有能力组件或者相应的GE为nullptr!"), *GetName());
		return;
	}

	auto GEContext = GetAbilitySystemComponent()->MakeEffectContext();
	GEContext.AddSourceObject(this);
	const auto GESpec = GetAbilitySystemComponent()->MakeOutgoingSpec(AttributesGameplayEffect, Level, GEContext);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GESpec.Data.Get(), GetAbilitySystemComponent());
}

void ACharacterBase::InitAllAttributes(bool BIsPlayer) 
{
	if (!GetAbilitySystemComponent())
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("能力系统为nullptr!"));
		return;
	}
	auto GiSubSystem = Cast<URPGAuraGameInstanceSubsystem>(
		USubsystemBlueprintLibrary::GetGameInstanceSubsystem(this, URPGAuraGameInstanceSubsystem::StaticClass()));
	if (!GiSubSystem)
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("获取GameInstance子系统失败!"));
	}
	GiSubSystem->InitializeDefaultAttributes(GetAbilitySystemComponent(), CharacterClass, GetCharacterLevel(), BIsPlayer);
}


bool ACharacterBase::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}

void ACharacterBase::InitAbilityActorInfo() {}

void ACharacterBase::HighLight()
{
	if (!CanHighLight()) { return; }
	Cast<IHighLightInterface>(this)->HighLightActor();
}

void ACharacterBase::UnHighLight()
{
	if (!CanHighLight()) { return; }
	Cast<IHighLightInterface>(this)->UnHighLightActor();
}

UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const { return AbilitySystemComponent; }
