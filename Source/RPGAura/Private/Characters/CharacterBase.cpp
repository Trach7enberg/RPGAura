// Copyright GGBAO 


#include "Characters/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "Interfaces/HighLightInterface.h"

DEFINE_LOG_CATEGORY_STATIC(ACharacterBaseLog, All, All);

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponLogicBaseComponent = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");


	if (GetMesh())
	{
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
}


void ACharacterBase::InitAttributes(const TSubclassOf<UGameplayEffect> AttributesGameplayEffect,
                                    const float Level) const
{
	if (!GetAbilitySystemComponent() || !AttributesGameplayEffect)
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("无法初始化属性，因为没有能力组件或者相应的GE为nullptr!"));
		return;
	}

	auto GEContext = GetAbilitySystemComponent()->MakeEffectContext();
	GEContext.AddSourceObject(this);
	const auto GESpec = GetAbilitySystemComponent()->MakeOutgoingSpec(AttributesGameplayEffect, Level, GEContext);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GESpec.Data.Get(), GetAbilitySystemComponent());
}


bool ACharacterBase::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}

void ACharacterBase::InitAbilityActorInfo()
{
}

void ACharacterBase::HighLight()
{
	if (!CanHighLight())
	{
		return;
	}
	Cast<IHighLightInterface>(this)->HighLightActor();
}

void ACharacterBase::UnHighLight()
{
	if (!CanHighLight())
	{
		return;
	}
	Cast<IHighLightInterface>(this)->UnHighLightActor();
}

UAbilitySystemComponent *ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
