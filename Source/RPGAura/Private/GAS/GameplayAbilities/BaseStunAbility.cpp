// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseStunAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseStunAbilityLog, All, All);

UBaseStunAbility::UBaseStunAbility() {}

void UBaseStunAbility::ApplyStun()
{
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->HasAuthority())
	{
		UE_LOG(UBaseStunAbilityLog, Error, TEXT("[%s]无权限!"), *GetName());
		return;
	}
	if (!StunEffectClass)
	{
		UE_LOG(UBaseStunAbilityLog, Error, TEXT("GE Class不能为nullptr!"));
		return;
	}

	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseStunAbilityLog, Error, TEXT("[%s:%s]Actor信息获取失败!"), *GetName(),
		       *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}

	auto GameplayEffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	const auto GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		StunEffectClass, GetAbilityLevel(), GameplayEffectContextHandle);
	// GetGrantedByEffectContext();
	// 设置动态持续时间
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle,FRPGAuraGameplayTags::Get().Abilities_DeBuff_Stun,StunDuration);
	StunActiveGameplayEffectHandle = ApplyGameplayEffectSpecToOwner(GetCurrentAbilitySpecHandle(),GetCurrentActorInfo(),GetCurrentActivationInfo(),GameplayEffectSpecHandle);
	
}

void UBaseStunAbility::RemoveStun()
{
	if (!GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseStunAbilityLog, Error, TEXT("[%s:%s]Actor信息获取失败!"), *GetName(),
		       *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}
	if (!GetAvatarActorFromActorInfo()->HasAuthority())
	{
		UE_LOG(UBaseStunAbilityLog, Error, TEXT("[%s:%s]无权限!"), *GetName(),
		       *FString::Printf(TEXT(__FUNCTION__)));
		return;
	}

	if (!StunActiveGameplayEffectHandle.IsValid())
	{
		UE_LOG(UBaseStunAbilityLog, Warning, TEXT("未启用相应的GE!"));
		return;
	}

	GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(
		StunActiveGameplayEffectHandle);
}

void UBaseStunAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo,
                                       const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	// 绑定眩晕标签移除时的回调函数 TODO 是否在蓝图显示调用,以提示使用者该能力通过其它方式自己结束? 
	if (GetAbilitySystemComponentFromActorInfo())
	{
		auto& OnGameplayEffectTagCountChange = GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(
			FRPGAuraGameplayTags::Get().Abilities_DeBuff_Stun);
		if (!OnGameplayEffectTagCountChange.IsBoundToObject(this))
		{
			OnGameplayEffectTagCountChange.AddUObject(this, &UBaseStunAbility::OnGrantedTag_Stun);
		}
		else { UE_LOG(UBaseStunAbilityLog, Error, TEXT("绑定过了！")); }
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBaseStunAbility::OnGrantedTag_Stun(const FGameplayTag Tag, int32 NewTagCount)
{
	if (NewTagCount <= 0)
	{
		UE_LOG(UBaseStunAbilityLog, Error, TEXT("眩晕结束"));
		RemoveStun();
		K2_EndAbility();
	}
}
