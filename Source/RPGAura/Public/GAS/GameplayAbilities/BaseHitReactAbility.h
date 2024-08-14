// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "BaseHitReactAbility.generated.h"

/**
 *  被击中时的反击能力基类
 */
UCLASS()
class RPGAURA_API UBaseHitReactAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseHitReactAbility();
	/// 执行被击中反应时GameplayEffect
	UFUNCTION(BlueprintCallable, Category="Apply")
	void ApplyHitReact();

	/// 取消被击中反应时GameplayEffect
	UFUNCTION(BlueprintCallable, Category="Apply")
	void RemoveHitReact();

protected:
	// 被击中反应GE实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> HitReactEffectClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	// 启用的HitReact GE效果
	FActiveGameplayEffectHandle HitReactActiveGameplayEffectHandle;
};
