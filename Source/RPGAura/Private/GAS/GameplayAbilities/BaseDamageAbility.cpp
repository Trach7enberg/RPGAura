// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseDamageAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraGasCoreTypes.h"
#include "GAS/Globals/GameAbilitySystemGlobals.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseDamageAbilityLog, All, All);

void UBaseDamageAbility::CauseDamage(TArray<AActor*> Targets)
{
	if (!Targets.Num()) { return; }

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

	// 创建GE 上下文
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();

	// 创建GE
	const auto GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	// 分配SetByCaller
	AssignTagSetByCallerMagnitudeWithDamageType(GameplayEffectSpecHandle, GetAbilityLevel());

	for (const auto& Actor : Targets)
	{
		const auto TargetActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
		if (!TargetActorAsc) { continue; }
		GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
			*GameplayEffectSpecHandle.Data.Get(), TargetActorAsc);
	}
}

void UBaseDamageAbility::AssignTagSetByCallerMagnitudeWithDamageType(const FGameplayEffectSpecHandle& SpecHandle, const float AbilityLevel) const
{
	const auto Context = UGameAbilitySystemGlobals::GetCustomGeContext(SpecHandle.Data.Get()->GetContext().Get());
	if (!Context||!DamageTypesMap.Num() || !SpecHandle.IsValid()  )
	{
		UE_LOG(UBaseDamageAbilityLog, Warning, TEXT("DamageTypesMap为空或者SepcHandle无效!"));
		return;
	}


	for (const auto& Pair : DamageTypesMap)
	{
		// 分配一个键值对给SetByCaller,键是游戏标签,值是设定的Magnitude,到时候在GE蓝图中选择我们分配的标签,该GE就会应用我们这里设定的Magnitude值
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,
		                                                              Pair.Key,
		                                                              Pair.Value.GetValueAtLevel(AbilityLevel));
		Context->AddDamageType(Pair.Key);
	}
}
