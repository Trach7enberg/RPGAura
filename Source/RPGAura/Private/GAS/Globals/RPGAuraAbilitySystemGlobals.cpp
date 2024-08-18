// Copyright GGBAO 


#include "GAS/Globals/RPGAuraAbilitySystemGlobals.h"

#include "CoreTypes/RPGAuraGasCoreTypes.h"

FGameplayEffectContext* URPGAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	
	return new FRPGAuraGameplayEffectContext();
}
