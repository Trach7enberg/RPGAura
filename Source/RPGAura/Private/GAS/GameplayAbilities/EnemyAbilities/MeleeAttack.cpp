// Copyright GGBAO 


#include "GAS/GameplayAbilities/EnemyAbilities/MeleeAttack.h"

#include "CoreTypes/RPGAuraCoreTypes.h"


DEFINE_LOG_CATEGORY_STATIC(UMeleeAttackLog, All, All);

FMontageWithTag UMeleeAttack::GetRandomAttackAnim(const TArray<FMontageWithTag> MontageWithTags)
{
	if (MontageWithTags.Num() == 0) { return FMontageWithTag(); }
	if (MontageWithTags.Num() == 1) { return MontageWithTags[0]; }
	return MontageWithTags[FMath::RandRange(0, MontageWithTags.Num() - 1)];
}
