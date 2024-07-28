// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 包含当前项目GameplayTags的结构体
 */
// USTRUCT(BlueprintType)
struct FRPGAuraGameplayTags
{
	// GENERATED_BODY()
	
	static const FRPGAuraGameplayTags &Get()
	{
		return GameplayTags;
	}

	/// 添加NativeGameplayTag,native意思是在Cpp中,即在Cpp中添加GameplayTag
	static void InitGameplayTags();

	FGameplayTag Attribute_Secondary_Strength;
	FGameplayTag Attribute_Secondary_Intelligence;
	FGameplayTag Attribute_Secondary_Resilience;
	FGameplayTag Attribute_Secondary_Vigor;

	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;

private:
	static FRPGAuraGameplayTags GameplayTags;
};
