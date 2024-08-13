// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

// 在GAS当中GE应用到玩家身上,并且获取资产标签时的委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetAssetTagsDelegate, const FGameplayTagContainer& /* AssetTags */);


/**
 * ASC
 */
UCLASS()
class RPGAURA_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
    GENERATED_BODY()

public:
    UBaseAbilitySystemComponent();

    virtual void BeginPlay() override;

    /// 初始化ASC中的一些设置、比如绑定回调函数
    virtual void InitSetting();

    // 应用GE时候 广播资产标签,用于弹出拾取信息
    FOnGetAssetTagsDelegate OnGetAssetTagsDelegate;

    /// 给玩家添加初始能力,可能有多个
    /// @param Abilities 能力列表
    void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities);

    /// 给玩家添加一个能力
    /// @param AbilityClass 能力类
    void AddCharacterAbility(const TSubclassOf<UGameplayAbility>& AbilityClass);

    /// 处理InputAction按下时的能力触发问题
    /// @param InputTag 输入的游戏标签
    void AbilityInputTagPressed(const FGameplayTag& InputTag);

    /// 处理InputAction持续按下时的能力触发问题
    /// @param InputTag 输入的游戏标签
    void AbilityInputTagHeld(const FGameplayTag& InputTag);

    /// 处理InputAction按键释放时的能力问题
    /// @param InputTag 输入的游戏标签
    void AbilityInputTagReleased(const FGameplayTag& InputTag);

    /// 根据游戏标签激活能力
    /// @param Tag 标签
    void TryActivateAbilityByTag(const FGameplayTag& Tag);
    
protected:
    /// 当前ACS被应用任意的GE到自己身上时触发的回调函数
    /// @param AbilitySystemComponent 
    /// @param GameplayEffectSpec 
    /// @param ActiveEffectHandle 
    UFUNCTION(Client, Reliable)
    void ClientOnGEAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
                                 const FGameplayEffectSpec& GameplayEffectSpec,
                                 FActiveGameplayEffectHandle ActiveEffectHandle);
};