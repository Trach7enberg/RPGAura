// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "BaseInShockAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGAURA_API UBaseInShockAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	/// 该事件响应当前能力的拥有者的(被)电击状态变化时的回调
	/// @param NewState 
	UFUNCTION(BlueprintImplementableEvent)
	void OnShockStateChange(bool NewState);
};
