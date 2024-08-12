// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RPGAuraGameInstanceSubsystem.generated.h"

class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataAssets|CharacterClassDefaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	/// 根据角色职业和当前角色是玩家还是NPC来初始化角色身上的属性值
	/// @param Asc 角色身上的AbilitySystemComponent
	/// @param CharacterClass 角色职业类别
	/// @param Level GE等级
	/// @param BIsPlayer 是否是玩家
	UFUNCTION(BlueprintCallable, Category="GAS")
	void InitializeDefaultAttributes( UAbilitySystemComponent* Asc, ECharacterClass CharacterClass, float Level , bool BIsPlayer = false);
};
