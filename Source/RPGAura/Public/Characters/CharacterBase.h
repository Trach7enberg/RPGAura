// Copyright GGBAO 

#pragma once

/**
 *	基础人物类
 */
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

class UWeaponLogicBaseComponent;

UCLASS(Abstract)
class RPGAURA_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/// 高亮当前角色
	virtual void HighLight();

	/// 解除高亮当前角色
	virtual void UnHighLight();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TObjectPtr<UWeaponLogicBaseComponent> WeaponLogicBaseComponent;

	/// 当前角色能否被高亮
	/// @return 能高亮则返回true
	virtual bool CanHighLight();

};
