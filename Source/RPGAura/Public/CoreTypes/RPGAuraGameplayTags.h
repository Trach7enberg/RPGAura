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

	FireResistance,
	PhysicalResistance,
	LightningResistance,
	ArcaneResistance,

	InputLMB,
	InputRMB,
	Input1,
	Input2,
	Input3,
	Input4,
	Input_Passive1,
	Input_Passive2,

	MaxHealth,
	MaxMana,

	Abilities_Damage_Spell_Fire,
	Abilities_Damage_Spell_Lightning,
	Abilities_Damage_Spell_Arcane,
	Abilities_Damage_Spell_Physical,

	Abilities_Effects_HitReact,

	Montage_Attack_Normal,
	Montage_Attack_LeftHand,
	Montage_Attack_RightHand,
	
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

	FGameplayTag Attribute_Main; // 用于表示属性添加面板中的客家属性点数行标签
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

	// 伤害抵抗属性
	FGameplayTag Attributes_Secondary_Resistance_Fire;
	FGameplayTag Attributes_Secondary_Resistance_Lightning;
	FGameplayTag Attributes_Secondary_Resistance_Arcane;
	FGameplayTag Attributes_Secondary_Resistance_Physical;

	// 元属性
	FGameplayTag Attributes_Meta_InComingDamage;
	FGameplayTag Attributes_Meta_InComingXP;

	FGameplayTag InputTag;	// 父节点标签
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	// 被动技能输入标签 TODO 同时也标识被动技能,待更改
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;
	

	// 用于GA的伤害类型, 法术伤害
	FGameplayTag Abilities_DamageType_Spell; // 父节点标签
	FGameplayTag Abilities_DamageType_Fire; // 火焰法术伤害
	FGameplayTag Abilities_DamageType_Lightning; // 雷电法术伤害
	FGameplayTag Abilities_DamageType_Arcane; // 奥术伤害
	FGameplayTag Abilities_DamageType_Physical; // 物理伤害

	// 和动画蒙太奇相关联的标签
	FGameplayTag CombatSocket_Normal;	// 普通攻击动画标签
	FGameplayTag CombatSocket_LeftHand;	// 左手攻击动画标签
	FGameplayTag CombatSocket_RightHand;	// 右手攻击动画标签

	// 启动攻击能力的标签
	FGameplayTag Abilities_Attack; // 父节点标签
	FGameplayTag Abilities_Attack_Melee;	// 近战标签
	FGameplayTag Abilities_Attack_Range;	// 远程标签
	FGameplayTag Abilities_Attack_Spell;	// 萨满法术攻击标签
	FGameplayTag Abilities_Attack_Summon;	// 萨满法术攻击标签

	// 用于标识法术装备菜单的当前能力技能的状态,一个能力同一时间只能有一种状态标签
	FGameplayTag Abilities_Status;	// 能力状态父标签
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	// 能力的分类,主动还是被动,无则是一些通用的能力,比如受击反应能力
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_Normal;

	// 实际的技能标签
	FGameplayTag Abilities_Attack_Spell_Fire_FireBolt; // 火系法术下的火球技能
	FGameplayTag Abilities_Attack_Spell_Lightning; // TODO 闪电系
	FGameplayTag Abilities_Attack_Spell_Arcane; // TODO 奥术系

	// 能力冷却的标签
	FGameplayTag Abilities_CoolDown_Fire_FireBolt; // 火球技能冷却时间标签
	
	// 简单的Actor标签
	FName Player = "Player";
	FName Enemy = "Enemy";


	// 受击反应
	FGameplayTag Abilities_Effects_HitReact;

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

	// 伤害类型映射到对应的抵抗类型的Map
	static TMap<FGameplayTag, FGameplayTag> DamageTypesToResistancesMap;

	// 包含当前项目所有标签的标签容器
	static FGameplayTagContainer GameplayTagsContainer;	// 包含需要用到的标签(几乎全部)的标签容器
	static FGameplayTagContainer VitalGameplayTagsContainer;	// 和属性集的重要属性一一对应的标签容器
	static FGameplayTagContainer PrimaryGameplayTagsContainer;	// 和属性集的主要属性一一对应的标签容器
	static FGameplayTagContainer SecondaryGameplayTagsContainer; // 和属性集的次要属性一一对应的标签容器
	static FGameplayTagContainer DamageTypesTagsContainer; // 所有伤害类型
	
	static FGameplayTagContainer AttackSpellFireTagsContainer; // 所有火系法术攻击技能的标签容器
	static FGameplayTagContainer AttackSpellLightningTagsContainer; // 所有闪电系法术攻击技能的标签容器
	static FGameplayTagContainer AttackSpellArcaneTagsContainer; // 所有奥术系法术攻击技能的标签容器
	static FGameplayTagContainer PassiveTagsContainer;	// 所有被动技能所对应的标签容器

private:
	// 存储所有游戏标签的静态类的实例
	static FRPGAuraGameplayTags GameplayTags;
};
