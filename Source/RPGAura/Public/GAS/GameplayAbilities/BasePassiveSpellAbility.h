// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "BasePassiveSpellAbility.generated.h"

/**
 * 基础(角色的)被动能力法术类
 * 被动技能启动或终止时候会自动播放以及结束对应的特效(必须得设置DefaultInputTag才生效)
 */
UCLASS()
class RPGAURA_API UBasePassiveSpellAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;
	
protected:
	virtual void ShowActivateVfx( const FGameplayTag& Tag);
	
private:
	UFUNCTION()
	void OnDeActivePassiveAbility(const FGameplayTag& PassiveAbilityTag);
};
