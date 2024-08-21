// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GameAbilitySystemGlobals.generated.h"

struct FRPGAuraGameplayEffectContext;
/**
 * 
 */
UCLASS()
class RPGAURA_API UGameAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

	static FRPGAuraGameplayEffectContext* GetCustomGeContext(FGameplayEffectContext* GeContextHandle);

	static void SetIsBlockedHit(FGameplayEffectContextHandle*GeContextHandle, bool bIsBlock);
	static void SetIsCriticalHit(FGameplayEffectContextHandle* GeContextHandle, bool bIsCriticalHit);

	static bool IsBlockedHit(const FGameplayEffectContextHandle& GeContextHandle);
	static bool IsCriticalHit(const FGameplayEffectContextHandle& GeContextHandle);
};
