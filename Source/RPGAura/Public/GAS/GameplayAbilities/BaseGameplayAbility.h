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
	UBaseGameplayAbility();
    // 每一个能力都有的初始默认能力标签 (该标签也用作默认输入键的标签识别)
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StartUp")
    FGameplayTag DefaultInputTag;
	
};