// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RPGAuraAbilitySystemGlobals.generated.h"

/**
 *  自定义的GAS全局类,该类定义了全局的变量供能力系统使用
 */
UCLASS()
class RPGAURA_API URPGAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

	UFUNCTION(BlueprintCallable, Category="GAS|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="GAS|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="GAS|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsBlockedHit);

	UFUNCTION(BlueprintCallable, Category="GAS|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsCriticalHit);
};
