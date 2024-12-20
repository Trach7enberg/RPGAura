#include "CoreTypes/RPGAuraGasCoreTypes.h"

#include "CoreTypes/RPGAuraGameplayTags.h"


bool FRPGAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	uint32 RepBits = 0;
	if (Ar.IsSaving())
	{
		if (bReplicateInstigator && Instigator.IsValid()) { RepBits |= 1 << 0; }
		if (bReplicateEffectCauser && EffectCauser.IsValid()) { RepBits |= 1 << 1; }
		if (AbilityCDO.IsValid()) { RepBits |= 1 << 2; }
		if (bReplicateSourceObject && SourceObject.IsValid()) { RepBits |= 1 << 3; }
		if (Actors.Num() > 0) { RepBits |= 1 << 4; }
		if (HitResult.IsValid()) { RepBits |= 1 << 5; }
		if (bHasWorldOrigin) { RepBits |= 1 << 6; }
		if (bIsBlockedHit) { RepBits |= 1 << 7; }
		if (bIsCriticalHit) { RepBits |= 1 << 8; }
		if (DamageTypes.Num() > 0) { RepBits |= 1 << 9; }
		if (DeBuffInfos.Num() > 0) { RepBits |= 1 << 10; }
		if (!Impulse.IsZero()) { RepBits |= 1 << 11; }
		if (bIsDeBuffSideEffect) { RepBits |= 1 << 12; }
		if (bIsKnockBackHit) { RepBits |= 1 << 13; }
		// 不为1才传输
		if (RadiusDamageFallOffFactor != 1) { RepBits |= 1 << 14; }
	}

	Ar.SerializeBits(&RepBits, 15);

	if (RepBits & (1 << 0)) { Ar << Instigator; }
	if (RepBits & (1 << 1)) { Ar << EffectCauser; }
	if (RepBits & (1 << 2)) { Ar << AbilityCDO; }
	if (RepBits & (1 << 3)) { Ar << SourceObject; }
	if (RepBits & (1 << 4)) { SafeNetSerializeTArray_Default<31>(Ar, Actors); }
	if (RepBits & (1 << 5))
	{
		if (Ar.IsLoading()) { if (!HitResult.IsValid()) { HitResult = TSharedPtr<FHitResult>(new FHitResult()); } }
		HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (RepBits & (1 << 6))
	{
		Ar << WorldOrigin;
		bHasWorldOrigin = true;
	}
	else { bHasWorldOrigin = false; }

	if (RepBits & (1 << 7)) { Ar << bIsBlockedHit; }
	if (RepBits & (1 << 8)) { Ar << bIsCriticalHit; }
	if (RepBits & (1 << 9)) { DamageTypes.NetSerialize(Ar, Map, bOutSuccess); }
	if (RepBits & (1 << 10))
	{
		// 网络序列化DeBuff信息数组
		SafeNetSerializeTArray_WithNetSerialize<31>(Ar, DeBuffInfos, Map);
	}
	if (RepBits & (1 << 11)) { Impulse.NetSerialize(Ar, Map, bOutSuccess); }
	if (RepBits & (1 << 12)) { Ar << bIsDeBuffSideEffect; }
	if (RepBits & (1 << 13)) { Ar << bIsKnockBackHit; }
	if (RepBits & (1 << 14)) { Ar << RadiusDamageFallOffFactor; }

	if (Ar.IsLoading())
	{
		AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
	// return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
}
