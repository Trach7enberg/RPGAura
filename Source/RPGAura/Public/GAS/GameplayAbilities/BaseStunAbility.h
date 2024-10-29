// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "BaseStunAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGAURA_API UBaseStunAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseStunAbility();

	/// 应用眩晕GE,眩晕GE是有duration的,不需要手动移除
	UFUNCTION(BlueprintCallable, Category="Apply")
	void ApplyStun();

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	float StunDuration = 3.f;
	
	// 眩晕GE实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> StunEffectClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

private:
	// 启用的HitReact GE效果
	FActiveGameplayEffectHandle StunActiveGameplayEffectHandle;

	/// 移除眩晕GE效果 TODO 眩晕GE是有周期的,该函数是否冗余...? 
	void RemoveStun();

	UFUNCTION()
	void OnGrantedTag_Stun(const FGameplayTag Tag, int32 NewTagCount);
};
