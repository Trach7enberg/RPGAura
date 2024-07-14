// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 *  基本玩家类
 */
UCLASS()
class RPGAURA_API ABasePlayerState : public APlayerState ,public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ABasePlayerState();

	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	// ~ IAbilitySystemInterface Begin
	virtual UAttributeSet *GetAttributeSet() const { return AttributeSet; }
	// ~ IAbilitySystemInterface End
	
protected:
	/// GAS的能力组件
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/// GAS的属性集
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
