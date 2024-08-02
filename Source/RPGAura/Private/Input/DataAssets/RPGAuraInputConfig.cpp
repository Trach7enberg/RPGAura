// Copyright GGBAO 


#include "Input/DataAssets/RPGAuraInputConfig.h"

DEFINE_LOG_CATEGORY_STATIC(URPGAuraInputConfigLog, All, All);

const UInputAction* URPGAuraInputConfig::FindInputAction(const FGameplayTag& InputTag, const bool BlogNotFound) const
{
    for (const auto& InputToGameplayTagStruct : AbilityInputActions)
    {
        if (InputToGameplayTagStruct.InputTag == InputTag && InputToGameplayTagStruct.InputAction)
        {
            return InputToGameplayTagStruct.InputAction;
        }
    }
    if (BlogNotFound)
    {
        UE_LOG(URPGAuraInputConfigLog, Warning, TEXT("没找到 InputTag %s 对应的InputAction!"), *InputTag.ToString());
    }
    return nullptr;
}