// Copyright GGBAO 


#include "Characters/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Data/CharacterClassInfo.h"
#include "Interfaces/HighLightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "UI/WidgetComponents/DamageTextComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ACharacterBaseLog, All, All);

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterLevel = 1;
	SelfLifeSpan = 1.f;
	BIsHitReacting = false;
	MaxWalkingSpeed = 600.f;
	bIsDie = false;
	CurrentSummonsCount = 0;
	MaxSummonsCount = 5;

	// 角色能改变导航网格,不会让一堆角色撞在一起
	SetCanAffectNavigationGeneration(true);

	WeaponLogicBaseComponent = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");
	DissolveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("DissolveTimelineComponent");
	SummonTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("SummonTimelineComponent");
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");

	if (GetMesh()) { GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); }
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponLogicBaseComponent);

	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;

	InitDissolveTimeLine();
	InitSummonTimeLine();
}

void ACharacterBase::AddCharacterAbilities()
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
	Asc->AddCharacterDefaultAbilities(GiSubSystem->CharacterClassInfo->CommonAbilities, GetCharacterLevel());

	const FCharacterClassDefaultInfo CcdI = GiSubSystem->CharacterClassInfo->FindClassDefaultInfo(CharacterClass);
	// 赋予相应角色的初始能力
	Asc->AddCharacterDefaultAbilities(
		CcdI.PrimaryStartUpAbilities,
		GetCharacterLevel());

	// 赋予并激活角色相应的初始被动能力
	Asc->AddCharacterDefaultAbilities(
		CcdI.StartUpPassiveAbilities,
		GetCharacterLevel(), true);
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

void ACharacterBase::InitAbilityActorInfo() {}

void ACharacterBase::RegisterGameplayTagEvent()
{
	// 绑定(注册)当ASC被授予Effects_HitReact标签或者(被完全)移除标签时触发的Event
	// ( EGameplayTagEventType::AnyCountChange 意味着任何改变都会触发,如果有多个相同的标签只移除一个也会被触发)
	GetAbilitySystemComponent()->RegisterGameplayTagEvent(FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact).
	                             AddUObject(
		                             this, &ACharacterBase::OnGrantedTag_HitReact);
}

bool ACharacterBase::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}


UNiagaraSystem* ACharacterBase::GetBloodEffect() { return BloodEffect; }
void ACharacterBase::StartSummonAnim() { StartSummonTimeline(); }

ECharacterClass ACharacterBase::GetCharacterClass()
{
	return CharacterClass;
}

FVector ACharacterBase::GetCombatSocketLocation(const FGameplayTag& GameplayTag)
{
	if (!GetMesh()) { return FVector::Zero(); }

	if (GameplayTag == FRPGAuraGameplayTags::Get().CombatSocket_Normal)
	{
		if (WeaponLogicBaseComponent->DoesNeedWeapon())
		{
			return WeaponLogicBaseComponent->GetWeaponSocketLocByName(
				WeaponLogicBaseComponent->GetWeaponTipSocketName());
		}
		return GetMesh()->GetSocketLocation(AttackSocketName_BodyTip);
	}
	if (GameplayTag == FRPGAuraGameplayTags::Get().CombatSocket_LeftHand)
	{
		return GetMesh()->GetSocketLocation(AttackSocketName_LeftHand);
	}
	if (GameplayTag == FRPGAuraGameplayTags::Get().CombatSocket_RightHand)
	{
		return GetMesh()->GetSocketLocation(AttackSocketName_RightHand);
	}
	return FVector::Zero();
}

void ACharacterBase::UpdateCharacterFacingTarget(const FVector& TargetLoc)
{
	MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(WarpTargetName, TargetLoc);
}

UAnimMontage* ACharacterBase::GetHitReactAnim() { return HitReactAnimMontage.Get(); }
UAnimMontage* ACharacterBase::GetDeathAnim() { return DeathAnimMontage; }
TArray<FMontageWithTag> ACharacterBase::GetAttackAnims() { return AttackMontageWithTagArray; }
UAnimMontage* ACharacterBase::GetSummonAnim() { return SummonAnimMontage; }

int32 ACharacterBase::GetMaxSummonsCount() { return MaxSummonsCount; }

int32 ACharacterBase::GetCurrentSummonsCount() { return CurrentSummonsCount; }

void ACharacterBase::UpdateCurrentSummonsCount(const int32 NewCount)
{
	CurrentSummonsCount = FMath::Clamp(CurrentSummonsCount + NewCount, 0, MaxSummonsCount);
	UE_LOG(ACharacterBaseLog, Error, TEXT("CurrentCoumt:[%d],MaxCount[%d]"), CurrentSummonsCount, MaxSummonsCount);
}

AActor* ACharacterBase::GetCombatTarget() { return nullptr; }

void ACharacterBase::SetCombatTarget(AActor* CombatTarget) {}

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
	// UE_LOG(ACharacterBaseLog, Warning, TEXT("销毁"));
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
	GetCharacterMovement()->MaxWalkSpeed = BIsHitReacting ? MaxWalkingSpeed / 2.f : MaxWalkingSpeed;

	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red,
	                                 FString::Printf(
		                                 TEXT("TagName: [%s] , NewTagCount: [%d]"), *Tag.ToString(), NewTagCount));
}

void ACharacterBase::SetDissolveMaterial()
{
	if (!DissolveMaterialInstanceCharacter || !DissolveMaterialInstanceWeapon)
	{
		UE_LOG(ACharacterBaseLog, Warning, TEXT("材质实例为nullptr!!"));
		return;
	}

	MaterialInstanceDynamic_Character = UMaterialInstanceDynamic::Create(DissolveMaterialInstanceCharacter, this);
	MaterialInstanceDynamic_Weapon = UMaterialInstanceDynamic::Create(DissolveMaterialInstanceWeapon, this);

	// TODO 角色的身上材质插槽可能有多个,目前全部只用同一个溶解材质
	for (int i = 0; i < GetMesh()->GetMaterials().Num(); ++i)
	{
		GetMesh()->SetMaterial(i, MaterialInstanceDynamic_Character);
	}
	WeaponLogicBaseComponent->SetWeaponMaterial(0, MaterialInstanceDynamic_Weapon);
}

void ACharacterBase::SetScalarParameterValue(
	UMaterialInstanceDynamic* MaterialInstance, const float Value, const FName ParameterName)
{
	MaterialInstance->SetScalarParameterValue(ParameterName, Value);
}


void ACharacterBase::InitDissolveTimeLine()
{
	if (!DissolveFloatCurve)
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("溶解时间线的浮点曲线表为空!!"));
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

void ACharacterBase::StartDissolveTimeline()
{
	if (!DissolveTimelineComponent || !GetMesh()) { return; }

	SetDissolveMaterial();

	// 动态材质实例没有设置 不允许播放时间线
	if (!MaterialInstanceDynamic_Character || !MaterialInstanceDynamic_Weapon) { return; }

	DissolveTimelineComponent->PlayFromStart();
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
	// SetLifeSpan(SelfLifeSpan);
	Destroy();
}

void ACharacterBase::InitSummonTimeLine()
{
	if (!SummonTimelineComponent) { return; }

	if (SummonFloatCurve)
	{
		FOnTimelineFloat OnUpdate;
		FOnTimelineEvent OnFinished;
		OnUpdate.BindDynamic(this, &ACharacterBase::SummonTimelineUpdateFunc);
		OnFinished.BindDynamic(this, &ACharacterBase::SummonTimelineFinishedFunc);

		SummonTimelineComponent->AddInterpFloat(SummonFloatCurve, OnUpdate, "Summon", "SummonTrack");
		SummonTimelineComponent->SetTimelineFinishedFunc(OnFinished);
		SummonTimelineComponent->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);
	}
	else { UE_LOG(ACharacterBaseLog, Warning, TEXT("召唤时间线的浮点曲线表为空!!")); }
}

void ACharacterBase::StartSummonTimeline() const
{
	if (!SummonTimelineComponent || !GetMesh() || !SummonFloatCurve) { return; }

	SummonTimelineComponent->PlayFromStart();
}

void ACharacterBase::SummonTimelineUpdateFunc(float Output) { GetMesh()->SetRelativeScale3D(FVector(Output)); }
void ACharacterBase::SummonTimelineFinishedFunc() {}

void ACharacterBase::Die()
{
	WeaponLogicBaseComponent->DetachWeapon();
	MulticastHandleDeath();
}

bool ACharacterBase::IsCharacterDie() { return bIsDie; }

void ACharacterBase::ShowDamageNumber_Implementation(const float Damage, bool bBlockedHit, bool bCriticalHit)
{
	if (!DamageTextComponentClass)
	{
		UE_LOG(ACharacterBaseLog, Warning, TEXT("无法弹出伤害显示文本!!"));
		return;
	}
	const auto DamageTextComponent = NewObject<UDamageTextComponent>(this, DamageTextComponentClass);

	// 如果是在构造函数创建的就不需要注册组件,CreateDefault默认帮我们做了,因此这里得手动注册
	DamageTextComponent->RegisterComponent();
	DamageTextComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageTextComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// Detach之后就会在原地
	// DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	const FText DamageStr = FText::FromString(FString::Printf(TEXT("%.3f"), Damage));

	FText HitMessageText = FText();
	if (bCriticalHit && bBlockedHit)
	{
		DamageTextComponent->SetDamageTextColor(FLinearColor::Yellow);
		DamageTextComponent->SetHitMessageTextColor(FLinearColor::Yellow);
		HitMessageText = FText::FromString(
			DamageTextComponent->HitMessage_Critical.ToString() + DamageTextComponent->HitMessage_Blocked.ToString() +
			"!");
	}
	else if (bCriticalHit)
	{
		DamageTextComponent->SetDamageTextColor(FLinearColor::Red);
		DamageTextComponent->SetHitMessageTextColor(FLinearColor::Red);
		HitMessageText = FText::FromString(DamageTextComponent->HitMessage_Critical.ToString() + "!");
	}
	else if (bBlockedHit)
	{
		DamageTextComponent->SetDamageTextColor(FLinearColor::Gray);
		DamageTextComponent->SetHitMessageTextColor(FLinearColor::Gray);
		HitMessageText = FText::FromString(DamageTextComponent->HitMessage_Blocked.ToString() + "!");
	}
	DamageTextComponent->SetDamageText(DamageStr);
	DamageTextComponent->SetHitMessageText(HitMessageText);
}


void ACharacterBase::MulticastHandleDeath_Implementation()
{
	bIsDie = true;

	if (DeathSound) { UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation()); }

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
