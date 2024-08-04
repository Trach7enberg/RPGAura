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

    /// 给玩家添加初始能力
    /// @param StartUpAbilities 能力列表
    void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);

    void AbilityInputTagPressed(const FGameplayTag& InputTag);
    void AbilityInputTagHeld(const FGameplayTag& InputTag);
    void AbilityInputTagReleased(const FGameplayTag& InputTag);

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