// Copyright GGBAO 

#pragma once

/**
 *	基础人物类
 */
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "CharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UWeaponLogicBaseComponent;


UCLASS(Abstract)
class RPGAURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
    GENERATED_BODY()

public:
    ACharacterBase();

    /// 高亮当前角色
    virtual void HighLight();

    /// 解除高亮当前角色
    virtual void UnHighLight();

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // ~ IAbilitySystemInterface
    virtual UAttributeSet* GetAttributeSet() const
    {
        return AttributeSet;
    }

    // ~ ICombatInterface
    virtual int32 GetCharacterLevel() override
    {
        return 0;
    };
    virtual FVector GetCombatSocketLocation() override;
    // ~ ICombatInterface

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditDefaultsOnly, Category="Weapon")
    TObjectPtr<UWeaponLogicBaseComponent> WeaponLogicBaseComponent;

    /// GAS的能力组件
    UPROPERTY()
    TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

    /// GAS的属性集
    UPROPERTY()
    TObjectPtr<UAttributeSet> AttributeSet;

    // 用于初始化角色主要属性的GE类
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GAS | Attributes")
    TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesGameplayEffect = nullptr;

    // 用于初始化角色次要属性的GE类,在主属性初始化后执行,该GE为infinite
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GAS | Attributes")
    TSubclassOf<UGameplayEffect> DefaultSecondaryPrimaryAttributesGameplayEffect = nullptr;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GAS | Abilities")
    TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

    /// 给角色授予能力(初始能力)
    void AddCharacterAbilities() const;
    
    /// 接受一个GE用来初始化角色身上的属性,次要属性必须得在主要属性初始化之后
    /// @param AttributesGameplayEffect GE类
    /// @param Level 应用GE的等级
    virtual void InitAttributes(TSubclassOf<UGameplayEffect> AttributesGameplayEffect, float Level = 1.f) const;

    /// 当前角色能否被高亮
    /// @return 能高亮则返回true
    virtual bool CanHighLight();

    virtual void InitAbilityActorInfo();
};