// Copyright GGBAO 


#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GAS/Data/AbilityDescriptionAsset.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "CoreTypes/RPGAuraGasCoreTypes.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Globals/GameAbilitySystemGlobals.h"
#include "Interfaces/CombatInterface.h"

DEFINE_LOG_CATEGORY_STATIC(URPGAuraBlueprintFunctionLibraryLog, All, All);

TObjectPtr<UAbilityDescriptionAsset> URPGAuraBlueprintFunctionLibrary::AbilityDescriptionAsset;

URPGAuraBlueprintFunctionLibrary::URPGAuraBlueprintFunctionLibrary() {}

void URPGAuraBlueprintFunctionLibrary::FindLivePlayersWithinRadius(const AActor* Causer,
                                                                   TArray<AActor*>& OutOverlappingActors,
                                                                   const TArray<AActor*>& IgnoreActors,
                                                                   const float Radius,
                                                                   const FVector& SphereOrigin, const bool IgnoreSelf,
                                                                   const FName IgnoreTag)
{
	if (!Causer || !IsValid(Causer))
	{
		UE_LOG(URPGAuraBlueprintFunctionLibraryLog, Warning, TEXT("Causer无效"));
		return;
	}

	FCollisionQueryParams SphereParams;
	if (IgnoreSelf) { SphereParams.AddIgnoredActor(Causer); }
	SphereParams.AddIgnoredActors(IgnoreActors);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(Causer, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity,
		                                FCollisionObjectQueryParams(
			                                FCollisionObjectQueryParams::InitType::AllDynamicObjects),
		                                FCollisionShape::MakeSphere(Radius), SphereParams);
	}

	// int LocalCount = LimitedNum;
	for (auto& OverlapResult : Overlaps)
	{
		// if(LocalCount < 0) { break; }
		// 忽视友军或敌人?
		if (IgnoreTag.IsValid() && OverlapResult.GetActor()->ActorHasTag(IgnoreTag)) { continue; }

		const auto DoseImplement = OverlapResult.GetActor()->Implements<UCombatInterface>();
		if (!DoseImplement) { continue; }

		const auto CombatInterface = Cast<ICombatInterface>(OverlapResult.GetActor());
		if (!CombatInterface) { continue; }

		const auto IsCharacterDie = CombatInterface->IsCharacterDie();
		if (!IsCharacterDie)
		{
			OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			// const auto IsAdd = OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			// if(IsAdd)
			// {
			// 	--LocalCount;
			// }
		}
	}
}

bool URPGAuraBlueprintFunctionLibrary::IsFriendly(const AActor* Actor1, AActor* Actor2)
{
	if (!Actor1 || !Actor2) { return false; }
	const auto bBothPlayers = Actor1->ActorHasTag(FRPGAuraGameplayTags::Get().Player) && Actor2->ActorHasTag(
		FRPGAuraGameplayTags::Get().Player);
	const auto bBothEnemies = Actor1->ActorHasTag(FRPGAuraGameplayTags::Get().Enemy) && Actor2->ActorHasTag(
		FRPGAuraGameplayTags::Get().Enemy);

	return bBothPlayers || bBothEnemies;
}

// UAbilityDescriptionAsset* URPGAuraBlueprintFunctionLibrary::GetAbilityDescriptionAsset()
// {
// 	return AbilityDescriptionAsset.Get();
// }

UAbilityDescriptionAsset* URPGAuraBlueprintFunctionLibrary::GetAbilityDescriptionAsset(UObject* Outer)
{
	if (!AbilityDescriptionAsset) { LoadAbilityDescriptionAsset(Outer); }
	return AbilityDescriptionAsset.Get();
}

FAbilityDescription URPGAuraBlueprintFunctionLibrary::GetAbilityDescription(
	AActor* Actor, UPARAM(ref) const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid() || !Actor) { return FAbilityDescription(); }
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor));

	if (!MyAsc) { return FAbilityDescription(); }

	return MyAsc->GetAbilityDescriptionByAbilityTag(AbilityTag);
}

void URPGAuraBlueprintFunctionLibrary::AssignTagSetByCallerMagnitudeWithDamageTypes(
	const TMap<FGameplayTag, FScalableFloat>& DamageTypesMap, const FGameplayEffectSpecHandle& SpecHandle,
	const float AbilityLevel)
{
	if (!SpecHandle.IsValid() || !SpecHandle.Data) { return; }

	const auto GeContext = UGameAbilitySystemGlobals::GetCustomGeContext(SpecHandle.Data.Get()->GetContext().Get());
	if (!GeContext || !DamageTypesMap.Num())
	{
		UE_LOG(URPGAuraBlueprintFunctionLibraryLog, Warning, TEXT("DamageTypesMap为空或者SepcHandle无效!"));
		return;
	}

	for (const auto& Pair : DamageTypesMap)
	{
		// 分配一个键值对给SetByCaller,键是游戏标签,值是设定的Magnitude,到时候在GE蓝图中选择我们分配的标签,该GE就会应用我们这里设定的Magnitude值
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,
		                                                              Pair.Key,
		                                                              Pair.Value.GetValueAtLevel(AbilityLevel));
		GeContext->AddDamageType(Pair.Key);
	}
}

void URPGAuraBlueprintFunctionLibrary::AssignTagSetByCallerMagnitudeByGeSpecHandle(
	const FGameplayEffectSpecHandle& SpecHandle,
	const FGameplayTagContainer& Tags,
	const TArray<float>& Magnitudes)
{
	if (!SpecHandle.IsValid() || !SpecHandle.Data) { return; }

	const auto GeContext = UGameAbilitySystemGlobals::GetCustomGeContext(SpecHandle.Data.Get()->GetContext().Get());
	if (!GeContext || !Tags.Num() || !Magnitudes.Num() || Tags.Num() != Magnitudes.Num())
	{
		// UE_LOG(URPGAuraBlueprintFunctionLibraryLog, Warning, TEXT("数组为空、幅度与标签数组长度不相同或者SepcHandle无效!"));
		return;
	}
	for (int i = 0; i < Tags.Num(); ++i)
	{
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,
		                                                              Tags.GetByIndex(i), Magnitudes[i]);
	}
}

FActiveGameplayEffectHandle URPGAuraBlueprintFunctionLibrary::ApplyDamageGameplayEffectByParams(
	const FDamageEffectParams& Params)
{
	if (!Params.IsParamsValid())
	{
		UE_LOG(URPGAuraBlueprintFunctionLibraryLog, Warning, TEXT("Params无效,无法应用伤害GE!"));
		return FActiveGameplayEffectHandle();
	}
	const auto SourceAvatar = Params.SourceAbilitySystemComponent->GetAvatarActor();
	if (!SourceAvatar) { return FActiveGameplayEffectHandle(); }

	auto EffectContextHandle = Params.SourceAbilitySystemComponent->MakeEffectContext();
	const auto GeSpecHandle = Params.SourceAbilitySystemComponent->MakeOutgoingSpec(
		Params.DamageGameplayEffectClass, Params.AbilityLevel, EffectContextHandle);
	EffectContextHandle.AddSourceObject(SourceAvatar);

	/// 传输设置冲击点的前向向量、以及范围衰减伤害系数(如果有)
	if (const auto MyGeContext = UGameAbilitySystemGlobals::GetCustomGeContext(EffectContextHandle.Get()))
	{
		MyGeContext->SetImpulse(Params.ImpulseVector * Params.KnockBackDirection);
		MyGeContext->SetRadiusDamageFallOffFactor(Params.RadiusDamageFallOffFactor);
	}
	AssignTagSetByCallerMagnitudeWithDamageTypes(Params.DamageTypesMap, GeSpecHandle, Params.AbilityLevel);
	AssignTagSetByCallerMagnitudeByGeSpecHandle(GeSpecHandle, FRPGAuraGameplayTags::Get().DeBuffEffectsTagsContainer,
	                                            TArray{
		                                            Params.DeBuffChance, Params.DeBuffDamage, Params.DeBuffDuration,
		                                            Params.DeBuffFrequency
	                                            });

	// 分配击退几率
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GeSpecHandle,
	                                                              FRPGAuraGameplayTags::Get().
	                                                              Abilities_SideEffect_KnockBack_Chance,
	                                                              Params.KnockBackChance);
	return Params.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GeSpecHandle.Data);
}

void URPGAuraBlueprintFunctionLibrary::FillDeBuffInfoFromGeSpec(const FGameplayTag& DamageTag,
                                                                const FGameplayTag& DeBuffTag, FDeBuffInfo& DeBuffInfo,
                                                                const FGameplayEffectSpec& GeSpec)
{
	if (!DamageTag.IsValid() || !DeBuffTag.IsValid()) { return; }

	DeBuffInfo.DamageType = DamageTag;
	DeBuffInfo.DeBuffType = DeBuffTag;
	DeBuffInfo.bIsSuccessfulDeBuff = true;

	DeBuffInfo.DeBuffDamage = GeSpec.GetSetByCallerMagnitude(
		FRPGAuraGameplayTags::Get().Abilities_DeBuff_Effects_Damage);
	DeBuffInfo.DeBuffDuration = GeSpec.GetSetByCallerMagnitude(
		FRPGAuraGameplayTags::Get().Abilities_DeBuff_Effects_Duration);
	DeBuffInfo.DeBuffFrequency = GeSpec.GetSetByCallerMagnitude(
		FRPGAuraGameplayTags::Get().Abilities_DeBuff_Effects_Frequency);
}

void URPGAuraBlueprintFunctionLibrary::GetVectorBySpread(const float BaseSpread, const float NumVector,
                                                         const FVector& ForwardVector, TArray<FVector>& OutArray,
                                                         const FVector& RotateAxis)
{
	if (NumVector <= 0) { return; }

	// 细分分子数
	const auto DeltaFactor = (FMath::IsNearlyZero((FMath::Fmod(BaseSpread, 360.f)))) ? 0 : 1;
	// 细分角度
	const auto DeltaSpread = (NumVector > 1) ? BaseSpread / (NumVector - DeltaFactor) : 0;
	// 初始向量的角度(以UpVector为轴)
	const auto InitSpread = (NumVector > 1) ? -((BaseSpread / 2.f)) : 0;
	// 左分散的向量
	const auto LeftOfSpread = ForwardVector.RotateAngleAxis(InitSpread, RotateAxis);

	for (int i = 0; i < NumVector; ++i)
	{
		const auto Dir = LeftOfSpread.RotateAngleAxis((DeltaSpread) * i,
		                                              RotateAxis);
		OutArray.Add(Dir);
	}
}

void URPGAuraBlueprintFunctionLibrary::LoadAbilityDescriptionAsset(UObject* Outer)
{
	AbilityDescriptionAsset = LoadObject<UAbilityDescriptionAsset>(
		Outer,TEXT(
			"/Script/RPGAura.AbilityDescriptionAsset'/Game/Blueprints/GAS/Data/DataAssets/DA_AbilitiesDescriptions.DA_AbilitiesDescriptions'"));
	if (!AbilityDescriptionAsset) { UE_LOG(URPGAuraBlueprintFunctionLibraryLog, Error, TEXT("获取技能描述数据资产失败!")); }
}

float URPGAuraBlueprintFunctionLibrary::GetRadialDamageWithFallOffFactor(const FVector& ActorLoc,
                                                                         const FVector& SphereCenter,
                                                                         const float OuterRadius,
                                                                         const float InnerRadius,
                                                                         const float MinDamageFactor,
                                                                         const float Tolerance)
{
	auto TmpInnerR = InnerRadius;

	// 没有内半径则当做普通圆处理
	if (TmpInnerR == 0) { TmpInnerR = OuterRadius; }
	if (OuterRadius < InnerRadius) { return 0.f; }

	// 内外半径一样,就当成没有内半径的一个普通圆
	const auto LocalInnerRadius = (TmpInnerR == OuterRadius) ? 0.f : TmpInnerR;

	// 圆心到Actor的距离
	auto Distance = FVector::Dist(SphereCenter, ActorLoc);
	const auto OuterRadiusTolerance = OuterRadius + Tolerance;

	// 处理当Actor在圆外不远处时,允许容差
	if (Distance > OuterRadius)
	{
		if (FMath::IsWithinInclusive(Distance, 0.f,OuterRadiusTolerance))
		{
			// 在容差范围内,直接将Actor的距离设置为球的最边界
			Distance = OuterRadius;
		}
	}

	// Actor在圆中,但是不在内半径时,即Actor在内半径之外小于等于外半径里 TODO 修改不会进入当前分支的bug 
	if (Distance > LocalInnerRadius && Distance <= OuterRadius)
	{
		const auto LinearFallOff = (1.f - ((Distance - LocalInnerRadius) / (OuterRadius -
			LocalInnerRadius)));
		return (MinDamageFactor + (1.f - MinDamageFactor) * LinearFallOff);
	}

	// Actor在内半径时
	if (Distance <= LocalInnerRadius) { return 1.f; }

	// Actor不在伤害圆中,因此没有伤害
	return 0.f;
}
