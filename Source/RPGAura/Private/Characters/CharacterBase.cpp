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
#include "UI/WidgetComponents/DamageTextComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ACharacterBaseLog, All, All);

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SelfLifeSpan = 1.f;
	BIsHitReacting = false;
	MaxWalkingSpeed = 600.f;

	WeaponLogicBaseComponent = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");
	DissolveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("DissolveTimelineComponent");

	if (GetMesh()) { GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); }
}


void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponLogicBaseComponent);

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;

	InitDissolveTimeLine();
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

void ACharacterBase::LifeSpanExpired()
{
	// 销毁武器,再销毁角色
	UE_LOG(ACharacterBaseLog, Warning, TEXT("销毁"));
	WeaponLogicBaseComponent->DestroyComponent(true);
	Super::LifeSpanExpired();
}

FORCEINLINE UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
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

void ACharacterBase::InitDissolveTimeLine()
{
	if (!DissolveFloatCurve)
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("时间线的浮点曲线表为空!!"));
		return;
	}

	FOnTimelineFloat OnUpdate;
	FOnTimelineEvent OnFinished;
	OnUpdate.BindDynamic(this, &ACharacterBase::DissolveTimelineUpdateFunc);
	OnFinished.BindDynamic(this, &ACharacterBase::DissolveTimelineFinishedFunc);

	DissolveTimelineComponent->AddInterpFloat(DissolveFloatCurve, OnUpdate, "Dissolve", "DissolveTrack");
	DissolveTimelineComponent->SetTimelineFinishedFunc(OnFinished);
	DissolveTimelineComponent->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
}


void ACharacterBase::DissolveTimelineUpdateFunc(float Output)
{
	SetScalarParameterValue(MaterialInstanceDynamic_Character, Output, ScalarParam);
	SetScalarParameterValue(MaterialInstanceDynamic_Weapon, Output, ScalarParam);
}

void ACharacterBase::DissolveTimelineFinishedFunc()
{
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red, FString::Printf(TEXT("TimelineFinished")));
	// 溶解动画完成,角色销毁
	SetLifeSpan(SelfLifeSpan);
}

void ACharacterBase::SetDissolveMaterial()
{
	if (!DissolveMaterialInstanceCharacter || !DissolveMaterialInstanceWeapon)
	{
		UE_LOG(ACharacterBaseLog, Warning, TEXT("材质实例为nullptr!!"));
		return;
	}

	MaterialInstanceDynamic_Character = UMaterialInstanceDynamic::Create(DissolveMaterialInstanceCharacter, this);
	GetMesh()->SetMaterial(0, MaterialInstanceDynamic_Character);


	MaterialInstanceDynamic_Weapon = UMaterialInstanceDynamic::Create(DissolveMaterialInstanceWeapon, this);
	WeaponLogicBaseComponent->SetWeaponMaterial(0, MaterialInstanceDynamic_Weapon);
}

void ACharacterBase::SetScalarParameterValue(
	UMaterialInstanceDynamic* MaterialInstance, const float Value, const FName ParameterName)
{
	MaterialInstance->SetScalarParameterValue(ParameterName, Value);
}


void ACharacterBase::StartDissolveTimeline()
{
	if (!DissolveTimelineComponent) { return; }

	SetDissolveMaterial();

	// 动态材质实例没有设置 不允许播放时间线
	if (!MaterialInstanceDynamic_Character || !MaterialInstanceDynamic_Weapon) { return; }

	DissolveTimelineComponent->PlayFromStart();
}

void ACharacterBase::Die()
{
	WeaponLogicBaseComponent->DetachWeapon();
	MulticastHandleDeath();
}

void ACharacterBase::ShowDamageNumber_Implementation(float Damage)
{
	if (!DamageTextComponentClass)
	{
		UE_LOG(ACharacterBaseLog, Warning, TEXT("无法弹出伤害显示文本!!"));
		return;
	}
	const auto DamageText = NewObject<UDamageTextComponent>(this, DamageTextComponentClass);

	// 如果是在构造函数创建的就不需要注册组件,CreateDefault默认帮我们做了,因此这里得手动注册
	DamageText->RegisterComponent();
	DamageText->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageText->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// Detach之后就会在原地
	// DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	DamageText->SetDamageText(Damage);
	
}


void ACharacterBase::MulticastHandleDeath_Implementation()
{
	// 设置武器物理开启
	WeaponLogicBaseComponent->SetWeaponPhysics(true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	// 启用布娃娃
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	// Pawn还可以进行物理互动
	// GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);


	// 播放溶解时间线动画,动画完成之后才死亡
	StartDissolveTimeline();
}
