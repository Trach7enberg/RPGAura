// Copyright GGBAO 


#include "GAS/Globals/RPGAuraAbilitySystemGlobals.h"

#include "CoreTypes/RPGAuraGasCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(URPGAuraAbilitySystemGlobalsLog, All, All);

FGameplayEffectContext* URPGAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRPGAuraGameplayEffectContext();
}

bool URPGAuraAbilitySystemGlobals::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const auto MyGEContext = static_cast<const FRPGAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (!MyGEContext)
	{
		UE_LOG(URPGAuraAbilitySystemGlobalsLog, Error, TEXT("获取GE上下文失败!!"));
		return false;
	}
	return MyGEContext->IsBlockedHit();
}

bool URPGAuraAbilitySystemGlobals::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	const auto MyGEContext = static_cast<const FRPGAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (!MyGEContext)
	{
		UE_LOG(URPGAuraAbilitySystemGlobalsLog, Error, TEXT("获取GE上下文失败!!"));
		return false;
	}
	return MyGEContext->IsCriticalHit();
}

void URPGAuraAbilitySystemGlobals::
SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool IsBlockedHit)
{
	const auto MyGEContext = static_cast<FRPGAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (MyGEContext)
	{
		MyGEContext->SetBIsBlockedHit(IsBlockedHit);
	}
}

void URPGAuraAbilitySystemGlobals::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
                                                    const bool IsCriticalHit)
{
	const auto MyGEContext = static_cast<FRPGAuraGameplayEffectContext*>(EffectContextHandle.Get());
	if (MyGEContext)
	{
		MyGEContext->SetBIsCriticalHit(IsCriticalHit);
	}
}
