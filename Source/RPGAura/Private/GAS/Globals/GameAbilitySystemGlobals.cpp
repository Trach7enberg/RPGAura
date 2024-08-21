// Copyright GGBAO 


#include "GAS/Globals/GameAbilitySystemGlobals.h"

#include "CoreTypes/RPGAuraGasCoreTypes.h"

FGameplayEffectContext* UGameAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRPGAuraGameplayEffectContext();
}

FRPGAuraGameplayEffectContext* UGameAbilitySystemGlobals::GetCustomGeContext(
	FGameplayEffectContext* GeContextHandle)
{
	return static_cast<FRPGAuraGameplayEffectContext*>(GeContextHandle);
}

void UGameAbilitySystemGlobals::SetIsBlockedHit(FGameplayEffectContextHandle* GeContextHandle,
                                                const bool bIsBlock)
{
	const auto MyGeContext = GetCustomGeContext(GeContextHandle->Get());
	if (MyGeContext) { return; }
	MyGeContext->SetBIsBlockedHit(bIsBlock);
}

void UGameAbilitySystemGlobals::SetIsCriticalHit(FGameplayEffectContextHandle* GeContextHandle,
                                                 const bool bIsCriticalHit)
{
	const auto MyGeContext = GetCustomGeContext(GeContextHandle->Get());
	if (MyGeContext) { return; }
	MyGeContext->SetBIsCriticalHit(bIsCriticalHit);
}

bool UGameAbilitySystemGlobals::IsBlockedHit(const FGameplayEffectContextHandle& GeContextHandle)
{
	const auto MyGeContext = static_cast<const FRPGAuraGameplayEffectContext*>(GeContextHandle.Get());
	
	if (MyGeContext) { return false; }
	return MyGeContext->IsBlockedHit();
}

bool UGameAbilitySystemGlobals::IsCriticalHit(const FGameplayEffectContextHandle& GeContextHandle)
{
	const auto MyGeContext = static_cast<const FRPGAuraGameplayEffectContext*>(GeContextHandle.Get());
	
	if (MyGeContext) { return false; }
	return MyGeContext->IsCriticalHit();
}
