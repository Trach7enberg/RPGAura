// Copyright GGBAO 


#include "Characters/CharacterBase.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "MotionWarpingComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/WeaponLogicBaseComponent.h"
#include "Controller/BasePlayerController.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameInstance/BaseGameInstance.h"
#include "GameModes/RPGAuraGameModeBase.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Data/CharacterClassInfo.h"
#include "GAS/Data/LootTiers.h"
#include "Interfaces/HighLightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup/BasePickup.h"
#include "PlayerStates/BasePlayerState.h"
#include "RPGAura/RPGAura.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "Subsystems/SubsystemBlueprintLibrary.h"
#include "UI/WidgetComponents/DamageTextComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ACharacterBaseLog, All, All);

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterLevel      = 1;
	SelfLifeSpan        = 0.1f;
	MaxWalkingSpeed     = 600.f;
	bIsDie              = false;
	CurrentSummonsCount = 0;
	MaxSummonsCount     = 5;

	// 角色能改变导航网格,不会让一堆角色撞在一起
	SetCanAffectNavigationGeneration(true);

	WeaponLogicBaseComponent  = CreateDefaultSubobject<UWeaponLogicBaseComponent>("WeaponLogicComponent");
	DissolveTimelineComponent = CreateDefaultSubobject<UTimelineComponent>("DissolveTimelineComponent");
	SummonTimelineComponent   = CreateDefaultSubobject<UTimelineComponent>("SummonTimelineComponent");
	MotionWarpingComponent    = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarping");
	NiagaraComponent          = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	NiagaraComponent->SetAutoActivate(false);

	// 设置不受贴花影响
	GetCapsuleComponent()->SetReceivesDecals(false);
	GetMesh()->SetReceivesDecals(false);

	if (GetMesh()) { GetMesh()->SetRelativeRotation(FRotator(0, -90, 0)); }
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	check(WeaponLogicBaseComponent);
	WeaponLogicBaseComponent->AttachWeaponToSocket(this, WeaponLogicBaseComponent->GetWeaponAttachSocketName());
	GetCharacterMovement()->MaxWalkSpeed = MaxWalkingSpeed;

	InitDissolveTimeLine();
	InitSummonTimeLine();
	LocalDefaultRootMotionMode = GetMesh()->AnimScriptInstance.Get()->RootMotionMode;
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
	                                  GetCharacterLevel(),
	                                  true);
}

void ACharacterBase::InitAttributes(const TSubclassOf<UGameplayEffect> AttributesGameplayEffect,
                                    const float                        Level) const
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
	                                                       USubsystemBlueprintLibrary::GetGameInstanceSubsystem(this,
		                                                       URPGAuraGameInstanceSubsystem::StaticClass()));
	if (!GiSubSystem) { UE_LOG(ACharacterBaseLog, Error, TEXT("获取GameInstance子系统失败!")); }
	GiSubSystem->InitializeDefaultAttributes(GetAbilitySystemComponent(),
	                                         CharacterClass,
	                                         GetCharacterLevel(),
	                                         BIsPlayer);
}

void ACharacterBase::InitAbilityActorInfo() {}

void ACharacterBase::RegisterGameplayTagEvent()
{
	if (!GetAbilitySystemComponent())
	{
		UE_LOG(ACharacterBaseLog, Error, TEXT("无法向能力组件注册标签事件回调!"));
		return;
	}
	// 绑定(注册)当ASC被授予Effects_HitReact标签或者(被完全)移除标签时触发的Event
	// ( EGameplayTagEventType::AnyCountChange 意味着任何改变都会触发,如果有多个相同的标签只移除一个也会被触发)
	GetAbilitySystemComponent()->RegisterGameplayTagEvent(FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact_Normal)
	                           .
	                           AddUObject(
	                                      this,
	                                      &ACharacterBase::OnGrantedTag_HitReact);

	GetAbilitySystemComponent()->RegisterGameplayTagEvent(FRPGAuraGameplayTags::Get().Abilities_DeBuff_Burn).AddUObject(
	 this,
	 &ACharacterBase::OnGrantedTag_DeBuffBurn);

	GetAbilitySystemComponent()->RegisterGameplayTagEvent(FRPGAuraGameplayTags::Get().Abilities_DeBuff_Stun).AddUObject(
	 this,
	 &ACharacterBase::OnGrantedTag_DeBuffStun);
}

ABasePlayerState* ACharacterBase::GetMyPlayerState()
{
	if (!BasePlayerState)
	{
		if (!GetPlayerState()) { return nullptr; }

		BasePlayerState = Cast<ABasePlayerState>(GetPlayerState());
	}
	return BasePlayerState;
}

URPGAuraGameInstanceSubsystem* ACharacterBase::GetMyGiSubSystem()
{
	if (!RPGAuraGameInstanceSubsystem)
	{
		if (!GetGameInstance()) { return nullptr; }

		RPGAuraGameInstanceSubsystem = GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	}
	return RPGAuraGameInstanceSubsystem;
}

ABasePlayerController* ACharacterBase::GetMyController()
{
	if (!BasePlayerController)
	{
		if (!GetController()) { return nullptr; }

		BasePlayerController = Cast<ABasePlayerController>(GetController());
	}
	return BasePlayerController.Get();
}

ARPGAuraGameModeBase* ACharacterBase::GetMyGameMode()
{
	if (!BaseGameMode)
	{
		const auto Gm = Cast<ARPGAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (!Gm) { return nullptr; }

		BaseGameMode = Gm;
	}
	return BaseGameMode.Get();
}

UBaseGameInstance* ACharacterBase::GetMyGi()
{
	if (!MyGi)
	{
		const auto Gi = GetGameInstance<UBaseGameInstance>();
		if (!Gi) { return nullptr; }

		MyGi = Gi;
	}
	return MyGi.Get();
}

void ACharacterBase::StopVfx(const FGameplayTag& Tag) { MulticastStopVfxWithTag(Tag); }

void ACharacterBase::MulticastStopVfxWithTag_Implementation(const FGameplayTag& Tag)
{
	if (const auto NComp = VfxComponentPool.Find(Tag)) { (*NComp)->Deactivate(); }
}

void ACharacterBase::MulticastStopVfx_Implementation()
{
	for (const auto& Pair : VfxComponentPool) { Pair.Value->DeactivateImmediate(); }
}

bool ACharacterBase::CanHighLight()
{
	const auto Can = Cast<IHighLightInterface>(this);

	return (Can) ? true : false;
}


UNiagaraSystem* ACharacterBase::GetBloodEffect() { return BloodEffect; }
void            ACharacterBase::StartSummonAnim() { StartSummonTimeline(); }

void ACharacterBase::ShowVfx(const FGameplayTag Tag)
{
	if (!Tag.IsValid()) { return; }
	if (const auto Vfx = DeBuffVfxMap.Find(Tag))
	{
		UE_LOG(ACharacterBaseLog, Warning, TEXT("[DeBuffVfx]: %s"), *Tag.GetTagName().ToString());
		// DeBuff特效是持续性的,不是一次性并且坐标是相对的不是世界系
		MulticastVfx(Tag,
		             Vfx->Get(),
		             {FRotator{}, FVector{}, FVector(DeBuffVfxScale)},
		             EAttachLocation::KeepRelativeOffset);
	}
}


void ACharacterBase::AddKnockBack(const FVector& Direction)
{
	if (!GetMesh() || !GetMesh()->AnimScriptInstance.Get()) { return; }

	// 说明角色正在被击退,不能再击退了防止叠加飞起来
	if (GetMesh()->AnimScriptInstance.Get()->RootMotionMode == ERootMotionMode::Type::NoRootMotionExtraction)
	{
		return;
	}

	if (!LandedDelegate.IsAlreadyBound(this, &ACharacterBase::OnKnockBackFinished))
	{
		LandedDelegate.AddDynamic(this, &ACharacterBase::OnKnockBackFinished);
	}
	GetMesh()->AnimScriptInstance.Get()->RootMotionMode = ERootMotionMode::Type::NoRootMotionExtraction;
	GetMovementComponent()->StopMovementImmediately();
	LaunchCharacter(FVector((Direction.X), (Direction.Y), 300), true, true);
}

void ACharacterBase::OnKnockBackFinished(const FHitResult& Hit)
{
	if (!GetMesh() || !GetMesh()->AnimScriptInstance.Get()) { return; }
	if (GetMesh()->AnimScriptInstance.Get()->RootMotionMode == ERootMotionMode::Type::NoRootMotionExtraction)
	{
		UE_LOG(ACharacterBaseLog, Warning, TEXT("[%s]击退结束"), *GetNameSafe(this));
		GetMesh()->AnimScriptInstance.Get()->RootMotionMode = LocalDefaultRootMotionMode;
	}
}

void ACharacterBase::SetCastShockAnimState(const bool Enabled) {}
bool ACharacterBase::GetCastShockAnimState() { return false; }
bool ACharacterBase::GetInShockHitState() { return BIsInShockHitReact; }

void ACharacterBase::SetInShockHitState(const bool Enabled)
{
	BIsInShockHitReact = Enabled;
	OnShockStateChangeSignature.Broadcast(BIsInShockHitReact);
}

USkeletalMeshComponent* ACharacterBase::GetWeaponMesh()
{
	if (!WeaponLogicBaseComponent) { return nullptr; }
	return WeaponLogicBaseComponent->GetWeaponMesh();
}

FOnDeathSignature&            ACharacterBase::GetPreOnDeathDelegate() { return OnDeathSignature; }
FOnShockStateChangeSignature& ACharacterBase::GetOnShockStateChangeDelegate() { return OnShockStateChangeSignature; }

void ACharacterBase::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!GetMyController()) { return; }
	GetMyController()->ShowMagicDecal();
	if (DecalMaterial) { GetMyController()->SetMagicDecalMaterial(DecalMaterial); }
}

void ACharacterBase::HideMagicCircle()
{
	if (!GetMyController()) { return; }
	GetMyController()->HideMagicDecal();
}

void ACharacterBase::SpawnLoot()
{
	if (!GetMyGiSubSystem() || !GetWorld()) { return; }

	const auto Asset = GetMyGiSubSystem()->GetLootTiersDataAsset();
	if (!Asset) { return; }

	TArray<FLootItem> LootItems{};
	Asset->GetLootItems(LootItems);

	SpawnLootDirections.Reset();
	SpawnLootDireIndex = 0;
	URPGAuraBlueprintFunctionLibrary::GetVectorBySpread(FMath::FRandRange(300, 360.f),
	                                                    LootItems.Num(),
	                                                    GetActorForwardVector(),
	                                                    SpawnLootDirections);
	if (SpawnLootDirections.Num() != LootItems.Num()) { return; }

	const auto     Distance = 200.f;
	FTimerDelegate SpawnLootDelegate{};

	const auto TmpActorLocation = GetActorLocation();

	SpawnLootDelegate.BindLambda([this,LootItems, TmpActorLocation]()
	{
		if (!IsValid(this) || !LootItems.IsValidIndex(SpawnLootDireIndex))
		{
			GetWorldTimerManager().ClearTimer(SpawnLootTimer);
			return;
		}

		const auto LocDistance  = FMath::FRandRange(50.f, 100.f);
		auto       CurrentPoint = SpawnLootDirections[SpawnLootDireIndex];
		FHitResult Hr{};
		UKismetSystemLibrary::LineTraceSingleByProfile(
		                                               this,
		                                               TmpActorLocation,
		                                               TmpActorLocation + CurrentPoint * LocDistance,
		                                               COLLISION_PRESET_DETECT_GROUND_POINTS,
		                                               false,
		                                               {},
		                                               EDrawDebugTrace::None,
		                                               Hr,
		                                               true,
		                                               FLinearColor::White,
		                                               FLinearColor::Red,
		                                               3.f
		                                              );

		// 修正点 TODO 需要修改垂直方向向量位置...  
		CurrentPoint = (Hr.bBlockingHit) ? Hr.ImpactPoint : TmpActorLocation + CurrentPoint * LocDistance;
		UE_LOG(ACharacterBaseLog, Warning, TEXT("%s 生成物下标[%d]"), TEXT(__FUNCTION__), SpawnLootDireIndex);
		FActorSpawnParameters SpawnParameters{};
		CurrentPoint.Z                                 = 100.f;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		if (auto Loot = GetWorld()->SpawnActor<ABasePickup>(LootItems[SpawnLootDireIndex].LootClass,
		                                                    CurrentPoint,
		                                                    CurrentPoint.Rotation(),
		                                                    SpawnParameters))
		{
			if (LootItems[SpawnLootDireIndex].bLootLevelOverride) { Loot->SetActorLevel(GetCharacterLevel()); }
			auto ImpulsiveDire = (Loot->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			ImpulsiveDire = ImpulsiveDire.RotateAngleAxis(FMath::FRandRange(45.f, 90.f), ImpulsiveDire.RightVector);
			auto Result = ImpulsiveDire * 400;
			Loot->EnablePickUpPhysics(Result);
		}
		SpawnLootDireIndex++;
	});

	GetWorldTimerManager().SetTimer(SpawnLootTimer, SpawnLootDelegate, .1f, true);
	for (int j = 0; j < LootItems.Num(); ++j)
	{
		// UKismetSystemLibrary::DrawDebugSphere(this,
		//                                       GetActorLocation() + Directions[j] * Distance,
		//                                       20,
		//                                       12,
		//                                       FLinearColor::Red,
		//                                       3,
		//                                       3);
	}
}

ECharacterClass ACharacterBase::GetCharacterClass() { return CharacterClass; }

FVector ACharacterBase::GetCombatSocketLocation(const FGameplayTag& GameplayTag)
{
	if (!GetMesh()) { return FVector::Zero(); }

	if (GameplayTag == FRPGAuraGameplayTags::Get().CombatSocket_Normal)
	{
		if (WeaponLogicBaseComponent->DoesNeedWeapon())
		{
			return WeaponLogicBaseComponent->GetWeaponSocketLocByName(
			                                                          WeaponLogicBaseComponent->
			                                                          GetWeaponTipSocketName());
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

UAnimMontage*           ACharacterBase::GetHitReactAnim() { return HitReactAnimMontage.Get(); }
UAnimMontage*           ACharacterBase::GetStunAnim() { return StunAnimMontage.Get(); }
UAnimMontage*           ACharacterBase::GetInShockAnim() { return HitReactLoopAnimMontage.Get(); }
UAnimMontage*           ACharacterBase::GetDeathAnim() { return DeathAnimMontage; }
TArray<FMontageWithTag> ACharacterBase::GetAttackAnims() { return AttackMontageWithTagArray; }
UAnimMontage*           ACharacterBase::GetSummonAnim() { return SummonAnimMontage; }

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
	// TODO 角色身上有Block标签也不会高亮

	Cast<IHighLightInterface>(this)->HighLightActor();
}

void ACharacterBase::UnHighLight()
{
	if (!CanHighLight()) { return; }
	Cast<IHighLightInterface>(this)->UnHighLightActor();
}

void ACharacterBase::Destroyed()
{
	// 销毁武器,再销毁角色
	// UE_LOG(ACharacterBaseLog, Warning, TEXT("角色销毁"));
	// WeaponLogicBaseComponent->DestroyComponent(true);
	Super::Destroyed();
}

void ACharacterBase::LifeSpanExpired()
{
	// 销毁武器,再销毁角色
	UE_LOG(ACharacterBaseLog, Warning, TEXT("角色销毁LifeSpanExpired"));
	WeaponLogicBaseComponent->DestroyComponent(true);
	Super::LifeSpanExpired();
}

void ACharacterBase::MulticastVfx_Implementation(const FGameplayTag&         VfxTag,
                                                 UNiagaraSystem*             Vfx,
                                                 const FTransform            VfxTransform,
                                                 const EAttachLocation::Type LocationType,
                                                 const bool                  SingleUse)
{
	if (!VfxTag.IsValid() || !Vfx || !NiagaraComponent) { return; }

	auto NiagaraComp = VfxComponentPool.Find(VfxTag);
	if (!NiagaraComp)
	{
		auto LocalNiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
		                                                                     Vfx,
		                                                                     GetRootComponent(),
		                                                                     NAME_None,
		                                                                     VfxTransform.GetLocation(),
		                                                                     VfxTransform.Rotator(),
		                                                                     LocationType,
		                                                                     SingleUse,
		                                                                     SingleUse);
		NiagaraComp = &LocalNiagaraComp;
		if (!SingleUse) { VfxComponentPool.Add(VfxTag, *NiagaraComp); }
	}

	if (NiagaraComp && (*NiagaraComp)) { (*NiagaraComp)->Activate(); }
}

FORCEINLINE UAbilitySystemComponent* ACharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


void ACharacterBase::OnGrantedTag_HitReact(const FGameplayTag Tag, int32 NewTagCount)
{
	GEngine->AddOnScreenDebugMessage(1,
	                                 2,
	                                 FColor::Red,
	                                 FString::Printf(
	                                                 TEXT("TagName: [%s] , NewTagCount: [%d]"),
	                                                 *Tag.ToString(),
	                                                 NewTagCount));
}

void ACharacterBase::OnGrantedTag_DeBuffBurn(const FGameplayTag Tag, int32 NewTagCount)
{
	const auto DeBuffing = NewTagCount > 0;

	// TODO 待封装为停止函数  
	if (!DeBuffing)
	{
		const auto TmpVfxComp = VfxComponentPool.Find(Tag);
		if (TmpVfxComp) { (*TmpVfxComp)->DeactivateImmediate(); }
		// TODO 需要判断是DeBuff特效,然后再停用DeBuff特效? 
		// MulticastStopVfx();
	}
	UE_LOG(ACharacterBaseLog,
	       Warning,
	       TEXT("获得DeBuff!,Actor: [%s] , DeBuff: [%s] , NewTagCount: [%d]"),
	       *GetNameSafe(this),
	       *Tag.ToString(),
	       NewTagCount);
}

void ACharacterBase::OnGrantedTag_DeBuffStun(const FGameplayTag Tag, int32 NewTagCount)
{
	const auto Stunning                  = NewTagCount > 0;
	const auto LocalWalkingSpeed         = (Stunning) ? 0.f : MaxWalkingSpeed;
	GetCharacterMovement()->MaxWalkSpeed = LocalWalkingSpeed;
	// 避免眩晕期间角色输入,让角色进行转向
	GetCharacterMovement()->bOrientRotationToMovement = !Stunning;

	if (!Stunning)
	{
		const auto TmpVfxComp = VfxComponentPool.Find(Tag);
		if (TmpVfxComp) { (*TmpVfxComp)->DeactivateImmediate(); }
		// TODO 需要判断是DeBuff特效,然后再停用DeBuff特效? 
		// MulticastStopVfx();
	}
	UE_LOG(LogTemp, Warning, TEXT("获得DeBuff![%s] -> 数量[%d]"), *Tag.ToString(), NewTagCount);
}

void ACharacterBase::SetDissolveMaterial()
{
	if (!DissolveMaterialInstanceCharacter || !DissolveMaterialInstanceWeapon)
	{
		UE_LOG(ACharacterBaseLog, Warning, TEXT("材质实例为nullptr!!"));
		return;
	}

	MaterialInstanceDynamic_Character = UMaterialInstanceDynamic::Create(DissolveMaterialInstanceCharacter, this);
	MaterialInstanceDynamic_Weapon    = UMaterialInstanceDynamic::Create(DissolveMaterialInstanceWeapon, this);

	// TODO 角色的身上材质插槽可能有多个,目前全部只用同一个溶解材质
	for (int i = 0; i < GetMesh()->GetMaterials().Num(); ++i)
	{
		GetMesh()->SetMaterial(i, MaterialInstanceDynamic_Character);
	}
	WeaponLogicBaseComponent->SetWeaponMaterial(0, MaterialInstanceDynamic_Weapon);
}

void ACharacterBase::SetScalarParameterValue(
	UMaterialInstanceDynamic* MaterialInstance,
	const float               Value,
	const FName               ParameterName) { MaterialInstance->SetScalarParameterValue(ParameterName, Value); }


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
	SetLifeSpan(SelfLifeSpan);
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

void ACharacterBase::Die(const FVector& Impulse, const bool IsFinalBlow) { MulticastHandleDeath(Impulse, IsFinalBlow); }

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
	DamageTextComponent->SetRelativeLocation(FVector(FMath::RandRange(10, 50),
	                                                 FMath::RandRange(10, 50),
	                                                 FMath::RandRange(10, 50)));
	// Detach之后就会在原地
	// DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	const FText DamageStr = FText::FromString(FString::Printf(TEXT("%.3f"), Damage));
	// TODO 伤害消息待设置为蓝图可配置,并且文字要支持本地化  
	FText HitMessageText = FText();
	if (bCriticalHit && bBlockedHit)
	{
		DamageTextComponent->SetDamageTextColor(FLinearColor::Yellow);
		DamageTextComponent->SetHitMessageTextColor(FLinearColor::Yellow);
		HitMessageText = FText::FromString(
		                                   DamageTextComponent->HitMessage_Critical.ToString() + DamageTextComponent->
		                                   HitMessage_Blocked.ToString() +
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


void ACharacterBase::MulticastHandleDeath_Implementation(const FVector& Impulse, const bool IsFinalBlow)
{
	bIsDie = true;
	OnDeathSignature.Broadcast(this);
	WeaponLogicBaseComponent->DetachWeapon();
	SpawnLoot();

	// TODO 已在多播函数内,待决定是否需要使用多播进行停止 
	MulticastStopVfx();
	// 启用布娃娃
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);

	if (DeathSound) { UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation()); }

	// 设置武器物理开启
	WeaponLogicBaseComponent->SetWeaponPhysics(true);
	// TODO 如果已经触发击退,则不应该添加死亡冲击? 
	if (IsFinalBlow) { AddDeathImpulse(Impulse); }

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	// Pawn还可以进行物理互动
	// GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);

	// 播放溶解时间线动画,动画完成之后才死亡
	StartDissolveTimeline();
}

void ACharacterBase::AddDeathImpulse_Implementation(const FVector& Impulse)
{
	auto LocalImpulse = Impulse;

	if (!bIsDie) { return; }
	if (GetMesh())
	{
		// TODO 待修改硬编码值,哥布林所需要的浮空力度与其他敌人不一样
		LocalImpulse.Z = 900;
		GetMesh()->AddImpulse(LocalImpulse * 5, NAME_None, true);
	}
	if (WeaponLogicBaseComponent) { WeaponLogicBaseComponent->AddWeaponImpulse(Impulse, NAME_None, true); }
}
