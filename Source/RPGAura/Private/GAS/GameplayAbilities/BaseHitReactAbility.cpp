// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseHitReactAbility.h"

#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseHitReactAbilityLog, All, All);


UBaseHitReactAbility::UBaseHitReactAbility() {}

void UBaseHitReactAbility::ApplyHitReact()
{
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->HasAuthority())
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("[ApplyHitReact]%s无权限!"), *GetName());
		return;
	}

	if (!HitReactEffectClass)
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("GE Class不能为nullptr!"));
		return;
	}
	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("[RemoveHitReact]%sActor信息获取失败!"), *GetName());
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
	if (!GetAvatarActorFromActorInfo())
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("[RemoveHitReact]%sActor信息获取失败!"), *GetName());
		return;
	}
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		UE_LOG(UBaseHitReactAbilityLog, Error, TEXT("[RemoveHitReact]%s无权限!"), *GetName());
		return;
	}

	if (!HitReactActiveGameplayEffectHandle.IsValid())
	{
		UE_LOG(UBaseHitReactAbilityLog, Warning, TEXT("未启用相应的GE!"));
		return;
	}

	GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(
		HitReactActiveGameplayEffectHandle);
	// BP_RemoveGameplayEffectFromOwnerWithHandle()
}

void UBaseHitReactAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
