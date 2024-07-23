// Copyright GGBAO 

#pragma once

/**
 *	基础人物类
 */
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UWeaponLogicBaseComponent;


UCLASS(Abstract)
class RPGAURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/// 高亮当前角色
	virtual void HighLight();

	/// 解除高亮当前角色
	virtual void UnHighLight();

	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	virtual UAttributeSet *GetAttributeSet() const
	{
		return AttributeSet;
	}

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GAS | Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesGameplayEffect = nullptr;

	/// 接受一个GE用来初始化角色身上的主要属性
	void InitPrimaryAttributes() const;

	/// 当前角色能否被高亮
	/// @return 能高亮则返回true
	virtual bool CanHighLight();

	virtual void InitAbilityActorInfo();

};
