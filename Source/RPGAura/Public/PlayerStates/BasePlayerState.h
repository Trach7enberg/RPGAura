// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "GameFramework/PlayerState.h"
#include "BasePlayerState.generated.h"

class URPGAuraGameInstanceSubsystem;
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

	/// 当前玩家经验值改变时的委托
	FOnPlayerInfoChangeSignature PlayerXpChangeDelegate;

	/// 当前玩家等级改变时的委托
	FOnPlayerInfoChangeSignature PlayerLevelChangeDelegate;

	virtual UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// ~ IAbilitySystemInterface Begin

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ~ IAbilitySystemInterface End

	/// 设置要进行网络复制的属性
	/// @param OutLifetimeProps 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE int32 GetPlayerLevel() const { return PlayerLevel; }

	FORCEINLINE int32 GetPlayerCurrentXP() const { return PlayerXP; }

	FORCEINLINE void SetPlayerLevel(const int32 NewPlayerLevel)
	{
		this->PlayerLevel = NewPlayerLevel;
		PlayerLevelChangeDelegate.Broadcast(PlayerLevel);
	}

	FORCEINLINE void SetPlayerXP(const int32 NewXp)
	{
		PlayerXP = NewXp;
		PlayerXpChangeDelegate.Broadcast(PlayerXP);
	}

	FORCEINLINE void AddToPlayerXP(const int32 AddedXp)
	{
		PlayerXP += AddedXp;
		PlayerXpChangeDelegate.Broadcast(PlayerXP);
	}

	FORCEINLINE void AddToPlayerLevel(const int32 AddedLevel)
	{
		PlayerLevel += AddedLevel;
		PlayerLevelChangeDelegate.Broadcast(PlayerLevel);
	}

	/// 获取给定等级能到达的最大经验值数
	/// @param TLevel
	/// @return 
	int32 GetMaximumXPofLevel(int32 TLevel);
	
	/// 获取升级到给定的等级所需要的最小经验数
	/// @param TLevel 
	/// @return 
	int32 GetMinimumXpRequiredForLevel(int32 TLevel) ;
	

protected:
	/// GAS的能力组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/// GAS的属性集
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AbilitySystem")
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	/// 玩家等级
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_PlayerLevel, Category = "PlayerLevel")
	int32 PlayerLevel = 1;

	// 默认经验为1级的经验,300
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_PlayerXP, Category = "PlayerLevel")
	int32 PlayerXP = 300;

	UPROPERTY()
	TObjectPtr<URPGAuraGameInstanceSubsystem> MyGiSubSystem = nullptr;
	
	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldValue);

	UFUNCTION()
	void OnRep_PlayerXP(int32 OldValue);

	URPGAuraGameInstanceSubsystem* GetGiSubSystem();
};
