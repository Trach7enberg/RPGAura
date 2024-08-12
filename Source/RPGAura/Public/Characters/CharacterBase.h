// Copyright GGBAO 

#pragma once

/**
 *	基础人物类
 */
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
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
	virtual UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// ~ ICombatInterface
	virtual int32 GetCharacterLevel() override { return 0; };
	virtual FVector GetCombatSocketLocation() override;

	virtual void UpdateCharacterFacingTarget(const FVector& TargetLoc) override {};
	// ~ ICombatInterface

protected:
	virtual void BeginPlay() override;

	// 当前角色的职业类别
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character Class Defaluts")
	ECharacterClass CharacterClass;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TObjectPtr<UWeaponLogicBaseComponent> WeaponLogicBaseComponent;

	/// GAS的能力组件
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/// GAS的属性集
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GAS | Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	/// 给角色授予能力(初始能力)
	void AddCharacterAbilities() const;

	/// 接受一个GE用来初始化角色身上的属性,次要属性必须得在主要属性初始化之后
	/// @param AttributesGameplayEffect GE类
	/// @param Level 应用GE的等级
	virtual void InitAttributes(TSubclassOf<UGameplayEffect> AttributesGameplayEffect, float Level = 1.f) const;

	/// 初始化角色身上的所有属性(职业对应的默认主要属性以及次要、vital属性),NPC和玩家的次要属性有所区分,不是同一种
	/// @param BIsPlayer 是否是玩家
	virtual void InitAllAttributes(bool BIsPlayer = false);

	/// 当前角色能否被高亮
	/// @return 能高亮则返回true
	virtual bool CanHighLight();

	virtual void InitAbilityActorInfo();
};
