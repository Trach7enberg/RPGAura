// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseHitReactAbility.h"

#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseHitReactAbilityLog, All, All);

void UBaseHitReactAbility::ApplyHitReact()
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) { return; }

	if (!HitReactEffectClass)
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("GE Class不能为nullptr!"));
		return;
	}
	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}
	
	auto GameplayEffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	const auto GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		HitReactEffectClass, GetAbilityLevel(), GameplayEffectContextHandle);

	HitReactActiveGameplayEffectHandle = ApplyGameplayEffectToOwner(GetCurrentAbilitySpecHandle(),
	                                                                GetCurrentActorInfo(), GetCurrentActivationInfo(),
	                                                                GameplayEffectSpecHandle.Data.Get()->Def,
	                                                                GetAbilityLevel());

	
}

void UBaseHitReactAbility::RemoveHitReact()
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}
	if (!HitReactActiveGameplayEffectHandle.IsValid())
	{
		UE_LOG(UBaseHitReactAbilityLog, Warning, TEXT("未启用相应的GE!"));
		return;
	}

	const auto Suc = GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(
		HitReactActiveGameplayEffectHandle);
	GEngine->AddOnScreenDebugMessage(1, 2, FColor::Red,
	                                 FString::Printf(
		                                 TEXT("移除效果:%d"), Suc));
}
