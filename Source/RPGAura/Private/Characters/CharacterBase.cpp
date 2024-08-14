// Copyright GGBAO 


#include "Characters/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Data/CharacterClassInfo.h"
#include "Interfaces/HighLightInterface.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(ACharacterBaseLog, All, All);

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SelfLifeSpan = 5.f;
	BIsHitReacting = false;
	MaxWalkingSpeed = 600.f;

	WeaponLogicBaseComponent = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");

	if (GetMesh()) { GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); }
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponLogicBaseComponent);

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;
}


void ACharacterBase::AddCharacterAbilities() const
{
	const auto GiSubSystem = GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!HasAuthority() || !GiSubSystem) { return; }

	if (!GiSubSystem->CharacterClassInfo)
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("[%s]初始能力对象为空!"), *GetName());
		return;
	}
	if (!GetAbilitySystemComponent()) { return; }

	const auto Asc = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!Asc) { return; }

	// 赋予角色的通用能力
	Asc->AddCharacterAbilities(GiSubSystem->CharacterClassInfo->CommonAbilities);

	// 赋予相应角色的初始能力
	Asc->AddCharacterAbilities(
		GiSubSystem->CharacterClassInfo->FindClassDefaultInfo(CharacterClass).PrimaryStartUpAbilities);
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
	if (!GiSubSystem) { UE_LOG(ACharacterBaseLog, Error, TEXT("获取GameInstance子系统失败!")); }
	GiSubSystem->InitializeDefaultAttributes(GetAbilitySystemComponent(), CharacterClass, GetCharacterLevel(),
	                                         BIsPlayer);
}


bool ACharacterBase::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}

void ACharacterBase::InitAbilityActorInfo() {}

void ACharacterBase::RegisterGameplayTagEvent()
{
	// 绑定(注册)当ASC被授予Effects_HitReact标签或者(被完全)移除标签时触发的Event
	// ( EGameplayTagEventType::AnyCountChange 意味着任何改变都会触发,如果有多个相同的标签只移除一个也会被触发)
	GetAbilitySystemComponent()->RegisterGameplayTagEvent(FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact).
	                             AddUObject(
		                             this, &ACharacterBase::OnGrantedTag_HitReact);
}

FVector ACharacterBase::GetCombatSocketLocation()
{
	return WeaponLogicBaseComponent->GetWeaponSocketLocByName(WeaponLogicBaseComponent->GetWeaponTipSocketName());
}

UAnimMontage* ACharacterBase::GetHitReactAnim() { return HitReactAnimMontage.Get(); }
UAnimMontage* ACharacterBase::GetDeathAnim() { return DeathAnimMontage; }

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

FORCEINLINE UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACharacterBase::MulticastHandleDeath_Implementation()
{
	WeaponLogicBaseComponent->SetWeaponPhysics(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	// 启用布娃娃
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	
}

void ACharacterBase::Die()
{
	WeaponLogicBaseComponent->DetachWeapon();
	MulticastHandleDeath();
	SetLifeSpan(SelfLifeSpan);
}

void ACharacterBase::OnGrantedTag_HitReact(const FGameplayTag Tag, int32 NewTagCount)
{
	BIsHitReacting = NewTagCount > 0;

	// 触发回调函数时,NewTagCount不大于0意味着当前标签正在移除,所以继续行走 否则停止走动
	GetCharacterMovement()->MaxWalkSpeed = BIsHitReacting ? 0.f : MaxWalkingSpeed;

	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red,
	                                 FString::Printf(
		                                 TEXT("TagName: [%s] , NewTagCount: [%d]"), *Tag.ToString(), NewTagCount));
}
