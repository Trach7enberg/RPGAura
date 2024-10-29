// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseInShockAbility.h"

#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "Interfaces/CombatInterface.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseInShockAbilityLog, All, All);

void UBaseInShockAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                          const FGameplayAbilityActorInfo* ActorInfo,
                                          const FGameplayAbilityActivationInfo ActivationInfo,
                                          const FGameplayEventData* TriggerEventData)
{
	if (GetAvatarActorFromActorInfo())
	{
		const auto ComBatIntF = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
		auto& Delegate = ComBatIntF->GetOnShockStateChangeDelegate();
		if (!Delegate.IsAlreadyBound(this, &UBaseInShockAbility::OnShockStateChange))
		{
			Delegate.AddDynamic(this, &UBaseInShockAbility::OnShockStateChange);
		}
		else { UE_LOG(UBaseInShockAbilityLog, Error, TEXT("[%s]绑定过了！"), *GetNameSafe(this)); }
	}
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
