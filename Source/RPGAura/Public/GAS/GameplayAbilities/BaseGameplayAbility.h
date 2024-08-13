// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "BaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class RPGAURA_API UBaseGameplayAbility : public UGameplayAbility
{
    GENERATED_BODY()

public:
    // 每一个能力都有的初始默认能力标签
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StartUp")
    FGameplayTag StartUpTag;

protected:

	// 根据CT缩放的伤害值
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Magnitude|Damage")
	FScalableFloat ScalableDamage;
};