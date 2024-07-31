// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FOnAttributeChangeData;
class UAttributeInfo;
struct FRPGAuraAttributeInfo;


/**
 *  属性菜单的控制器类
 */
UCLASS()
class RPGAURA_API UAttributeMenuWidgetController : public UBaseWidgetController
{
    GENERATED_BODY()

public:
    virtual void BroadcastInitialValues() override;
    virtual void BindCallBack() override;

    /// 属性值变化时的委托
    UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
    FOnAttributeChangedSignature OnAttributeChanged;

    /// 游戏标签到属性的映射数据资产表信息
    /// @return 映射数据资产表信息
    TObjectPtr<UAttributeInfo> GetAttributeInfoAsset() const
    {
        return AttributeInfo;
    }

protected:
    /// 游戏标签到属性的映射数据资产表信息
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
    TObjectPtr<UAttributeInfo> AttributeInfo = nullptr;

private:
    /// 绑定当属性集中的主要、次要属性变化时调用的回调函数
    /// @param Data FOnAttributeChangeData
    void HandleAnyAttributeChange(const FOnAttributeChangeData& Data) const;

    void BroadcastAttributeInfoStruct(const UBaseAttributeSet* MyAs, const TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair) const;

    void HandleAnyAttributeChange(const UBaseAttributeSet* MyAs, const TTuple<FGameplayTag, FGameplayAttribute(*)()>& Pair) const;


    /// 根据TagArray广播属性结构信息
    /// @param MyAs 属性集
    /// @param TagArray 标签数组
    void DoFirstBroadcast(const UBaseAttributeSet* MyAs, const TArray<FGameplayTag*>& TagArray) const;
};