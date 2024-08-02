// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "RPGAuraInputConfig.generated.h"

USTRUCT(BlueprintType)
struct FRPGAuraInputToGameplayTag
{
    GENERATED_BODY()


    UPROPERTY(EditDefaultsOnly, meta=(description="输入动作"))
    const class UInputAction* InputAction = nullptr;

    UPROPERTY(EditDefaultsOnly, meta=(description="输入动作对应的游戏标签"))
     FGameplayTag InputTag = FGameplayTag();
};

/**
 *  输入动作映射到游戏标签的数据资产类
 */
UCLASS()
class RPGAURA_API URPGAuraInputConfig : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TArray<FRPGAuraInputToGameplayTag> AbilityInputActions;

    /// 根据游戏标签查找输入动作(InputAction)
    /// @param InputTag 游戏标签
    /// @param BlogNotFound 是否打印
    /// @return 找到的输入动作 
    const UInputAction* FindInputAction(const FGameplayTag& InputTag, bool BlogNotFound = true) const;
};


