// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseDamageAbilityLog, All, All);

void UBaseDamageAbility::CauseDamage(AActor* Suffer)
{
	const auto TargetActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Suffer);
	if (!TargetActorAsc) { return; }

	if (!GetAvatarActorFromActorInfo()->HasAuthority()) { return; }

	if (!DamageEffectClass)
	{
		UE_LOG(UBaseDamageAbilityLog, Error, TEXT("造成伤害的GE Class不能为nullptr!"));
		return;
	}
	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseDamageAbilityLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}

	FDamageEffectParams EffectParams;
	EffectParams.ImpulseVector = GetOwningActorFromActorInfo()->GetActorForwardVector().RotateAngleAxis(
			45.f, GetOwningActorFromActorInfo()->GetActorRightVector());
	MakeDamageEffectParamsFromAbilityDefaults(EffectParams,Suffer);
	URPGAuraBlueprintFunctionLibrary::ApplyDamageGameplayEffectByParams(EffectParams);
	
}

FMontageWithTag UBaseDamageAbility::GetRandomAttackAnim(const TArray<FMontageWithTag> MontageWithTags)
{
	if (MontageWithTags.Num() == 0) { return FMontageWithTag(); }
	if (MontageWithTags.Num() == 1) { return MontageWithTags[0]; }
	return MontageWithTags[FMath::RandRange(0, MontageWithTags.Num() - 1)];
}

void UBaseDamageAbility::AssignTagSetByCallerMagnitudeWithDamageType(const FGameplayEffectSpecHandle& SpecHandle,
                                                                     const float AbilityLevel) const
{
	URPGAuraBlueprintFunctionLibrary::AssignTagSetByCallerMagnitudeWithDamageTypes(
		DamageTypesMap, SpecHandle, AbilityLevel);
}

float UBaseDamageAbility::GetEstimatedDamageFromDamageTypesMap(const int32 AbilityLevel) const
{
	float Damage = 0.f;
	for (auto& Pair : DamageTypesMap) { Damage += Pair.Value.GetValueAtLevel(AbilityLevel); }
	return Damage;
}

void UBaseDamageAbility::MakeDamageEffectParamsFromAbilityDefaults(FDamageEffectParams &Params,AActor* TargetActor) const
{
	Params.BaseDamage = GetEstimatedDamageFromDamageTypesMap(GetAbilityLevel());
	Params.DeBuffChance = DeBuffChance;
	Params.KnockBackChance = KnockBackChance;
	Params.DeBuffDamage = DeBuffDamage;
	Params.DeBuffDuration = DeBuffDuration;
	Params.DeBuffFrequency = DeBuffFrequency;
	Params.AbilityLevel = GetAbilityLevel();

	Params.DamageTypesMap = DamageTypesMap;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
}
 