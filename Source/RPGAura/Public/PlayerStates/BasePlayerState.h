// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "GameFramework/PlayerState.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
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

	/// 当前玩家可分配的属性点改变时的委托
	FOnPlayerInfoChangeSignature AssignableAttributePointsChangeDelegate;

	/// 当前玩家可分配的技能点改变时的委托
	FOnPlayerInfoChangeSignature AssignableSpellPointsChangeDelegate;

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

	 void SetPlayerXP(const int32 NewXp);

	 void AddToPlayerXP(const int32 AddedXp);

	FORCEINLINE void AddToPlayerLevel(const int32 AddedLevel)
	{
		PlayerLevel += AddedLevel;
		PlayerLevelChangeDelegate.Broadcast(PlayerLevel);
	}

	FORCEINLINE int32 GetAssignableAttributePoints() const { return AssignableAttributePoints; }

	FORCEINLINE void SetAssignableAttributePoints(const int32 NewAssignableAttributePoints)
	{
		this->AssignableAttributePoints = FMath::Max(NewAssignableAttributePoints,0);
		AssignableAttributePointsChangeDelegate.Broadcast(AssignableAttributePoints);
	}

	FORCEINLINE int32 GetAssignableSpellPoints() const { return AssignableSpellPoints; }

	FORCEINLINE void SetAssignableSpellPoints(const int32 NewAssignableSpellPoints)
	{
		this->AssignableSpellPoints = NewAssignableSpellPoints;
		AssignableSpellPointsChangeDelegate.Broadcast(AssignableSpellPoints);
	}

	 void AddToAssignableAttributePoints(const int32 AddedValue);

	FORCEINLINE void AddToAssignableSpellPoints(const int32 AddedValue)
	{
		AssignableSpellPoints = FMath::Max(AssignableSpellPoints + AddedValue, 0);
		AssignableSpellPointsChangeDelegate.Broadcast(AssignableSpellPoints);
	}

	/// 获取给定等级能到达的最大经验值数
	/// @param TLevel
	/// @return 
	int32 GetMaximumXPofLevel(int32 TLevel);

	/// 获取升级到给定的等级所需要的最小经验数
	/// @param TLevel 
	/// @return 
	int32 GetMinimumXpRequiredForLevel(int32 TLevel);

	/// 获取角色升级到对应等级所能获得到的奖励属性点数
	/// @param CharacterLevel
	/// @param MultipleLevel 从CharacterLevel开始,依次递减获取MultipleLevel次的奖励点数,大于1时才有意义
	/// @return 角色当前等级所能获得到的总奖励属性点数(如果MultipleLevel > 1 否则只获取对应等级的奖励点数)
	int32 GetAttributePointsReward(int32 CharacterLevel, int32 MultipleLevel = 1);

	/// 获取角色升级到对应等级所能获得到的技能点数奖励
	/// @param CharacterLevel
	/// @param MultipleLevel 
	/// @return 
	int32 GetSpellPointsReward(int32 CharacterLevel, int32 MultipleLevel);

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

	/// 当前角色可以分配的属性点
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_AssignableAttributePoints, Category = "PlayerLevel")
	int32 AssignableAttributePoints = 0;

	/// 当前角色可以分配的法术点
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_AssignableSpellPoints, Category = "PlayerLevel")
	int32 AssignableSpellPoints = 0;


	UPROPERTY()
	TObjectPtr<URPGAuraGameInstanceSubsystem> MyGiSubSystem = nullptr;

	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldValue);

	UFUNCTION()
	void OnRep_PlayerXP(int32 OldValue);

	UFUNCTION()
	void OnRep_AssignableAttributePoints(int32 OldValue);

	UFUNCTION()
	void OnRep_AssignableSpellPoints(int32 OldValue);

	URPGAuraGameInstanceSubsystem* GetGiSubSystem();
};
