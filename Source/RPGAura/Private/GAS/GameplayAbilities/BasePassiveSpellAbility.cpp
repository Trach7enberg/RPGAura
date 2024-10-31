// Copyright GGBAO 


#include "GAS/GameplayAbilities/BasePassiveSpellAbility.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "Interfaces/CombatInterface.h"

DEFINE_LOG_CATEGORY_STATIC(UBasePassiveSpellAbilityLog, All, All);

void UBasePassiveSpellAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                               const FGameplayAbilityActorInfo* ActorInfo,
                                               const FGameplayAbilityActivationInfo ActivationInfo,
                                               const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	if (!GetAbilitySystemComponentFromActorInfo()) { return; }

	if (const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		if (auto& Delegate = MyAsc->OnDeActivePassiveAbility; !Delegate.IsBoundToObject(this))
		{
			Delegate.AddUObject(this, &UBasePassiveSpellAbility::OnDeActivePassiveAbility);
		}
	}

	ShowActivateVfx(DefaultInputTag);
}

void UBasePassiveSpellAbility::ShowActivateVfx(const FGameplayTag& Tag)
{
	if (!GetAvatarActorFromActorInfo()) { return; }

	const auto CombIntF = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombIntF) { return; }

	CombIntF->ShowVfx(Tag);
}

void UBasePassiveSpellAbility::OnDeActivePassiveAbility(const FGameplayTag& PassiveAbilityTag)
{
	if (AbilityTags.HasTagExact(PassiveAbilityTag))
	{
		if (GetAvatarActorFromActorInfo())
		{
			if (const auto CombIntF = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
			{
				CombIntF->StopVfx(DefaultInputTag);
			}
		}
		K2_EndAbility();
	}
}
