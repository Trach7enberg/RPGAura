// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/EnemyAbilities/BaseEnemyAttack.h"
#include "MeleeAttack.generated.h"

struct FMontageWithTag;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class RPGAURA_API UMeleeAttack : public UBaseEnemyAttack
{
	GENERATED_BODY()


public:
	/// 获取随机的攻击动画
	/// @param MontageWithTags 
	/// @return 
	UFUNCTION(BlueprintCallable,Category="Combat")
	FMontageWithTag GetRandomAttackAnim(const TArray<FMontageWithTag> MontageWithTags);
};
