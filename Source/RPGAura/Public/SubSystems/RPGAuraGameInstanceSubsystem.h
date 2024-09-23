// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGAuraGameInstanceSubsystem.generated.h"


class ULevelUpInfoAsset;
class UTagToAbilityInfoAsset;
class UPickupMessageAsset;
class UCharacterClassInfo;
/**
 * GameInstance子系统类
 */
UCLASS()
class RPGAURA_API URPGAuraGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/// 属性值变化时的委托
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChangedSignature OnAttributeChanged;

	// 输入键与对应绑定的能力变化或初始化时的委托
	UPROPERTY(BlueprintAssignable, Category="GAS | Abilities")
	FAbilityInfoSignature AbilityInfoDelegate;

	// 游戏角色职业信息资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataAssets|CharacterClassDefaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	// 玩家等级信息资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataAssets|Player")
	TObjectPtr<ULevelUpInfoAsset> LevelUpInfoAsset;

	// 消息数据资产,不同的Tag对应不同的消息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataAsset")
	TObjectPtr<UPickupMessageAsset> MessageWidgetDataAsset ;


		
	/// 根据角色职业和当前角色是玩家还是NPC来初始化角色身上的属性值
	/// @param Asc 角色身上的AbilitySystemComponent
	/// @param CharacterClass 角色职业类别
	/// @param Level GE等级
	/// @param BIsPlayer 是否是玩家
	UFUNCTION(BlueprintCallable, Category="GAS")
	void InitializeDefaultAttributes( UAbilitySystemComponent* Asc, ECharacterClass CharacterClass, float Level , bool BIsPlayer = false);

	/// 从CharacterClassInfo资产中根据角色职业和等级获取相应等级的Xp奖励
	/// @param CharacterClass 
	/// @param CharacterLevel 
	/// @return 
	UFUNCTION(BlueprintCallable,Category="Level")
	int32 GetXpRewardFromClassAndLevel(ECharacterClass CharacterClass,int32 CharacterLevel) const;

	/// 根据给定的XP值返回从数据资产中找到所对应的等级
	/// 找不到则返回角色当前等级
	/// @param CharacterClass 
	/// @param CharacterXP
	/// @param CharacterLevel
	/// @return 
	UFUNCTION(BlueprintCallable,Category="Level")
	int32 GetLevelCorrespondingToXP(ECharacterClass CharacterClass,int32 CharacterXP,int32 CharacterLevel) const;

	/// 获取给定职业能达到的最大角色等级
	/// @return 
	UFUNCTION(BlueprintCallable,Category="Level")
	int32 GetCharacterDefaultMaxLevel();

	/// 获取给定职业能达到的最大角色经验
	/// @return 
	UFUNCTION(BlueprintCallable,Category="Level")
	int32 GetCharacterDefaultMaxXP();

	/// 获取AbilityInfo数据资产,结果可能为nullptr
	/// @param WorldContextObject 
	/// @return 
	static UTagToAbilityInfoAsset* GetAbilityInfoAsset(const UObject* WorldContextObject);
	
protected:
	// 技能信息资产,其中部分信息由GameMode设置
	static TObjectPtr<UTagToAbilityInfoAsset> AbilityInfoAsset ;
};
