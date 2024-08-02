// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/DataAssets/RPGAuraInputConfig.h"
#include "BaseEnhancedInputComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseEnhancedInputComponentLog,All,All);

/**
 * 本项目的基础增强输入组件
 */
UCLASS()
class RPGAURA_API UBaseEnhancedInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()

public:
    /// 将输入绑定到GAS
    /// @tparam UserClass 用户类,即绑定函数的类
    /// @tparam PressedFuncType 按下按键时的回调函数(Started) 
    /// @tparam ReleasedFuncType 释放按键时的回调函数(Completed)
    /// @tparam HeldFuncType 持续按住按键时的回调函数(Triggered)
    /// @param InputConfig 保存着InputAction映射到GameplayTag的数据资产表类
    /// @param Object 用户类
    /// @param PressedFunc 按下按键时的回调函数
    /// @param ReleasedFunc 释放按键时的回调函数
    /// @param HeldFunc 持续按住按键时的回调函数
    template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
    void BindAbilityActions(const URPGAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UBaseEnhancedInputComponent::BindAbilityActions(const URPGAuraInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
    if (!InputConfig)
    {
        UE_LOG(UBaseEnhancedInputComponentLog, Error, TEXT("InputConfig为空!!"));
        return;
    }

    for (const auto& Element : InputConfig->AbilityInputActions)
    {
        if (!Element.InputAction || !Element.InputTag.IsValid())
        {
            continue;
        }

        if (PressedFunc)
        {
            BindAction(Element.InputAction, ETriggerEvent::Started, Object, PressedFunc, Element.InputTag);
        }
        if (ReleasedFunc)
        {
            BindAction(Element.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Element.InputTag);
        }
        if (HeldFunc)
        {
            BindAction(Element.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Element.InputTag);
        }
    }
}