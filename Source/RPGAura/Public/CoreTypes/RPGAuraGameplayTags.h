// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGAuraCoreTypes.h"

/// GAS的属性集里属性、InputAction 对应的游戏标签
UENUM(BlueprintType)
enum class EGameplayTagNum : uint8
{
	CurrentHealth,
	CurrentMana,

	Strength,
	Intelligence,
	Resilience,
	Vigor,

	Armor,
	ArmorPenetration,
	BlockChance,
	CriticalHitChance,
	CriticalHitResistance,
	CriticalHitDamage,
	HealthRegeneration,
	ManaRegeneration,
	PhysicalResistance,
	MagicalResistance,

	InputLMB,
	InputRMB,
	Input1,
	Input2,
	Input3,
	Input4,

	MaxHealth,
	MaxMana,

	Event_Montage_FireBolt,

	Damage,

	Abilities_Effects_HitReact,
};


/**
 * 包含当前项目GameplayTags的结构体
 */
// USTRUCT(BlueprintType)
struct FRPGAuraGameplayTags
{
	// GENERATED_BODY()

	/// 添加NativeGameplayTag,native意思是在Cpp中,即在Cpp中添加GameplayTag
	static void InitGameplayTags();

	FGameplayTag Attribute_Vital_CurrentHealth;
	FGameplayTag Attribute_Vital_CurrentMana;

	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;

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

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Event_Montage_FireBolt;

	FGameplayTag Abilities_Damage_FireBolt;
	
	FGameplayTag Abilities_Effects_HitReact;

	
	// 标签的真实名字
	FString CurrentHealth = "Attributes.Vital.CurrentHealth";
	FString CurrentMana = "Attributes.Vital.CurrentMana";

	FString Strength = "Attributes.Primary.Strength";
	FString Intelligence = "Attributes.Primary.Intelligence";
	FString Resilience = "Attributes.Primary.Resilience";
	FString Vigor = "Attributes.Primary.Vigor";

	FString Armor = "Attributes.Secondary.Armor";
	FString ArmorPenetration = "Attributes.Secondary.ArmorPenetration";
	FString BlockChance = "Attributes.Secondary.BlockChance";
	FString CriticalHitChance = "Attributes.Secondary.CriticalHitChance";
	FString CriticalHitResistance = "Attributes.Secondary.CriticalHitResistance";
	FString CriticalHitDamage = "Attributes.Secondary.CriticalHitDamage";
	FString HealthRegeneration = "Attributes.Secondary.HealthRegeneration";
	FString ManaRegeneration = "Attributes.Secondary.ManaRegeneration";
	FString PhysicalResistance = "Attributes.Secondary.PhysicalResistance";
	FString MagicalResistance = "Attributes.Secondary.MagicalResistance";

	FString InputLMB = "InputTag.LMB";
	FString InputRMB = "InputTag.RMB";
	FString Input1 = "InputTag.1";
	FString Input2 = "InputTag.2";
	FString Input3 = "InputTag.3";
	FString Input4 = "InputTag.4";

	/// 获取GameplayTags静态实例
	/// @return GameplayTags单例
	static const FRPGAuraGameplayTags& Get() { return GameplayTags; }

	/// 根据游戏标签的类型(全部、重要、主要、次要)返回对应的标签容器
	/// @param TagType 游戏标签的类型(Vital、Primary、Secondary、还是所有标签)
	/// @return 游戏标签数组的指针
	static const FGameplayTagContainer& GetTagsContainerByType(const EGameplayTagType TagType);


	static EGameplayTagNum* FindEnumByTag(const FGameplayTag& GameplayTag);
	

	// 标签映射到枚举的Map
	static TMap<FGameplayTag, EGameplayTagNum> TagToNumMap;

	// 包含当前项目所有标签的标签容器
	static FGameplayTagContainer GameplayTagsContainer;
	static FGameplayTagContainer VitalGameplayTagsContainer;
	static FGameplayTagContainer PrimaryGameplayTagsContainer;
	static FGameplayTagContainer SecondaryGameplayTagsContainer;

private:
	// 存储所有游戏标签的静态类的实例
	static FRPGAuraGameplayTags GameplayTags;
	
};
