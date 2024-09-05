// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGAuraGameInstanceSubsystem.generated.h"


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

	// 技能信息资产
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataAssets|AbilityInfos")
	TObjectPtr<UTagToAbilityInfoAsset> AbilityInfoAsset;
	
	/// 根据角色职业和当前角色是玩家还是NPC来初始化角色身上的属性值
	/// @param Asc 角色身上的AbilitySystemComponent
	/// @param CharacterClass 角色职业类别
	/// @param Level GE等级
	/// @param BIsPlayer 是否是玩家
	UFUNCTION(BlueprintCallable, Category="GAS")
	void InitializeDefaultAttributes( UAbilitySystemComponent* Asc, ECharacterClass CharacterClass, float Level , bool BIsPlayer = false);
	

	// 消息数据资产,不同的Tag对应不同的消息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataAsset")
	TObjectPtr<UPickupMessageAsset> MessageWidgetDataAsset ;
};
