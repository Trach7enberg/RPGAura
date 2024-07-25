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
class RPGAURA_API ABasePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABasePlayerState();

	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;

	// ~ IAbilitySystemInterface Begin
	virtual UAttributeSet *GetAttributeSet() const
	{
		return AttributeSet;
	}

	// ~ IAbilitySystemInterface End

	/// 设置要进行网络复制的属性
	/// @param OutLifetimeProps 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	FORCEINLINE int32 GetPlayerLevel() const
	{
		return PlayerLevel;
	}

	FORCEINLINE void SetPlayerLevel(const int32 NewPlayerLevel)
	{
		this->PlayerLevel = NewPlayerLevel;
	}

protected:
	/// GAS的能力组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/// GAS的属性集
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	/// 玩家等级
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_PlayerLevel, Category = "PlayerLevel")
	int32 PlayerLevel = 1;

	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldValue);

};
