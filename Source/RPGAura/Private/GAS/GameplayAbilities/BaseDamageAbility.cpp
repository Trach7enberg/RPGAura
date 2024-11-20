// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseDamageAbilityLog, All, All);

void UBaseDamageAbility::CauseDamage(AActor* Suffer, FDamageEffectParams& EffectParams)
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

	// 以当前Instigator的方向为冲击向量,如果冲击向量没有被设置
	if (EffectParams.ImpulseVector.IsNearlyZero())
	{
		EffectParams.ImpulseVector = GetDefaultImpulseVector(KnockBackFactor);
	}
	MakeDamageEffectParamsFromAbilityDefaults(EffectParams, Suffer);
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

void UBaseDamageAbility::MakeDamageEffectParamsFromAbilityDefaults(FDamageEffectParams& Params,
                                                                   AActor* TargetActor) const
{
	Params.BaseDamage = Params.RadiusDamageFallOffFactor * GetEstimatedDamageFromDamageTypesMap(GetAbilityLevel());
	Params.DeBuffChance = DeBuffChance;
	Params.KnockBackChance = KnockBackChance;
	Params.KnockBackFactor = KnockBackFactor;
	Params.KnockBackDirection = KnockBackDirection;
	Params.DeBuffDamage = DeBuffDamage;
	Params.DeBuffDuration = FMath::Max(1, DeBuffDuration - 1);	// TODO DeBuff的持续时间减一才合适,待修复  
	Params.DeBuffFrequency = DeBuffFrequency;
	Params.AbilityLevel = GetAbilityLevel();

	Params.DamageTypesMap = DamageTypesMap;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
}

FVector UBaseDamageAbility::GetDefaultImpulseVector(const float ImpulseFactor, const float ImpulseAngle,
                                                    const float Direction) const
{
	if (!GetAvatarActorFromActorInfo()) { return FVector{}; }
	return (GetAvatarActorFromActorInfo()->GetActorForwardVector()).RotateAngleAxis(
		(-ImpulseAngle) * Direction, GetAvatarActorFromActorInfo()->GetActorRightVector()) * ImpulseFactor * Direction;
}

void UBaseDamageAbility::UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel)
{
	if (!CurrentAbilityDescription.IsDescriptionValid()) { return; }
	const auto DescStrNormal = CurrentAbilityDescription.DescriptionNormal.ToString();
	const auto DescStrLocked = CurrentAbilityDescription.DescriptionLocked.ToString();
	const auto DescStrNextLevel = CurrentAbilityDescription.DescriptionNextLevel.ToString();

	const auto FormatNormalStr = FString::Format(*DescStrNormal, {
		                                             // 当前技能等级{0}
		                                             AbilityLevel,
		                                             // 能力冷却时间{1}
		                                             FString::Printf(TEXT("%.1f"), GetCoolDown(AbilityLevel)),
		                                             // 多少个技能产生的物体{2}
		                                             GetValidAbilityCount(AbilityLevel),
		                                             // 预计伤害{3}
		                                             FString::Printf(
			                                             TEXT("%.1f"),
			                                             GetEstimatedDamageFromDamageTypesMap(AbilityLevel)),
		                                             // DeBuff几率{4}
		                                             FString::Printf(TEXT("%.1f"), DeBuffChance),
		                                             // DeBuff持续时间{5}
		                                             FString::Printf(TEXT("%.1f"), DeBuffDuration),
		                                             // DeBuff伤害{6}
		                                             FString::Printf(TEXT("%.1f"), DeBuffDamage),
		                                             // 能力蓝耗值{7}
		                                             FString::Printf(
			                                             TEXT("%.1f"),
			                                             FMath::Abs(GetManaCost(AbilityLevel))),
	                                             });

	const auto NextLevel = AbilityLevel + 1;
	const auto FormatNextLevelStr = FString::Format(*DescStrNextLevel, {
		                                                // 当前技能等级{0}
		                                                NextLevel,
		                                                // 能力冷却时间{1}
		                                                FString::Printf(TEXT("%.1f"), GetCoolDown(NextLevel)),
		                                                // 多少个技能产生的物体{2}
		                                                GetValidAbilityCount(AbilityLevel) ,
		                                                // 预计伤害{3}
		                                                FString::Printf(
			                                                TEXT("%.1f"),
			                                                GetEstimatedDamageFromDamageTypesMap(NextLevel)),
		                                                // DeBuff几率{4}
		                                                FString::Printf(TEXT("%.1f"), DeBuffChance),
		                                                // DeBuff持续时间{5}
		                                                FString::Printf(TEXT("%.1f"), DeBuffDuration),
		                                                // DeBuff伤害{6}
		                                                FString::Printf(TEXT("%.1f"), DeBuffDamage),
		                                                // 能力蓝耗值{7}
		                                                FString::Printf(
			                                                TEXT("%.1f"),
			                                                FMath::Abs(GetManaCost(NextLevel))),
	                                                });
	CurrentAbilityDescription.DescriptionNormal = FText::FromString(FormatNormalStr);
	CurrentAbilityDescription.DescriptionNextLevel = FText::FromString(FormatNextLevelStr);
}

int32 UBaseDamageAbility::GetValidAbilityCount(const int32 AbilityLevel)
{
	return FMath::Min(AbilityLevel, MaxAbilityCount);
}
