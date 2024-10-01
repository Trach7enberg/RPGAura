// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "BaseGameplayAbility.generated.h"


/**
 * 基础能力类
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
	
    /// 获取当前能力的详细描述
    /// @param AbilityTag 能力标签
    /// @param AbilityLevel
    /// @return
    virtual FAbilityDescription GetAbilityDescription(const FGameplayTag& AbilityTag,
                                                      int32 AbilityLevel);

protected:
    /// 当前能力的详细描述
    UPROPERTY()
	FAbilityDescription CurrentAbilityDescription = FAbilityDescription();

    /// 更新当前能力的详细描述
    /// 子类要更改对应技能的描述就覆盖此函数并且设置CurrentAbilityDescription即可
    /// @param AbilityTag 
    /// @param AbilityLevel 
    virtual void UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel);


    
};