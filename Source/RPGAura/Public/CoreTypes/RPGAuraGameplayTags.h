// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGAuraCoreTypes.h"

/// GAS的属性集里属性、InputAction 对应的游戏标签
UENUM(BlueprintType)
enum class EGameplayTagNum
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

	Abilities_DeBuff_Burn,
	Abilities_DeBuff_Stun,
	Abilities_DeBuff_Arcane,
	Abilities_DeBuff_Physical,
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

	FGameplayTag InputTag; // 父节点标签
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	// 被动技能输入标签 
	FGameplayTag InputTag_Passive_1;
	FGameplayTag InputTag_Passive_2;


	// 用于GA的伤害类型, 法术伤害
	FGameplayTag Abilities_DamageType_Spell;     // 父节点标签
	FGameplayTag Abilities_DamageType_Fire;      // 火焰法术伤害
	FGameplayTag Abilities_DamageType_Lightning; // 雷电法术伤害
	FGameplayTag Abilities_DamageType_Arcane;    // 奥术伤害
	FGameplayTag Abilities_DamageType_Physical;  // 物理伤害

	// 和动画蒙太奇相关联的标签
	FGameplayTag CombatSocket_Normal;    // 普通攻击动画标签
	FGameplayTag CombatSocket_LeftHand;  // 左手攻击动画标签
	FGameplayTag CombatSocket_RightHand; // 右手攻击动画标签

	// 启动攻击能力的标签
	FGameplayTag Abilities_Attack;        // 父节点标签
	FGameplayTag Abilities_Attack_Melee;  // 近战标签
	FGameplayTag Abilities_Attack_Range;  // 远程标签
	FGameplayTag Abilities_Attack_Spell;  // 萨满法术攻击标签
	FGameplayTag Abilities_Attack_Summon; // 萨满法术攻击标签

	// 实际的技能标签 ,分为火、电、奥术三系
	FGameplayTag Abilities_Attack_Spell_Fire_FireBolt;         // 火系法术下的火球技能
	FGameplayTag Abilities_Attack_Spell_Fire_BlazeBurst;       // 火系法术下的焰爆扩散技能
	FGameplayTag Abilities_Attack_Spell_Lightning_Electrocute; // 雷电系法术下的触电技能
	FGameplayTag Abilities_Attack_Spell_Arcane_ArcaneShards;   // TODO 奥术系

	// 能力的负面效果 
	FGameplayTag Abilities_DeBuff;
	FGameplayTag Abilities_DeBuff_Burn;     // 火焰伤害附带的DeBuff
	FGameplayTag Abilities_DeBuff_Stun;     // 闪电伤害附带的
	FGameplayTag Abilities_DeBuff_Arcane;   // 奥术伤害附带的
	FGameplayTag Abilities_DeBuff_Physical; // 物理伤害附带

	// 用于标识能力的特效 TODO 除了Misc其它的暂时未用到,直接用的DeBuff标签 
	FGameplayTag Abilities_Vfx;
	FGameplayTag Abilities_VfxDeBuff_Burn;
	FGameplayTag Abilities_VfxDeBuff_Stun;
	FGameplayTag Abilities_VfxDeBuff_Arcane;
	FGameplayTag Abilities_VfxDeBuff_Physical;
	FGameplayTag Abilities_Vfx_Misc_LevelUp; // 升级特效

	// 负面效果的各种效果,主要用于GE的SetByCaller来调整相应的magnitude
	FGameplayTag Abilities_DeBuff_Effects;
	FGameplayTag Abilities_DeBuff_Effects_Chance;
	FGameplayTag Abilities_DeBuff_Effects_Damage;
	FGameplayTag Abilities_DeBuff_Effects_Duration;  // DeBuff的持续时间
	FGameplayTag Abilities_DeBuff_Effects_Frequency; // GE 中的 "period",在持续时间内的触发周期

	// 能力的"副作用"
	FGameplayTag Abilities_SideEffect;
	FGameplayTag Abilities_SideEffect_KnockBack_Chance; // 能力触发击退的概率 

	// 用于标识法术装备菜单的当前能力技能的状态,一个能力同一时间只能有一种状态标签
	FGameplayTag Abilities_Status; // 能力状态父标签
	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Passive;
	FGameplayTag Abilities_Passive_HaloOfProtection; // 保护光环
	FGameplayTag Abilities_Passive_LifeSiphon;       // 生命虹吸被动
	FGameplayTag Abilities_Passive_ManaSiphon;       // 法术虹吸被动

	// 能力的分类,主动还是被动,无则是一些通用的能力,比如受击反应能力
	FGameplayTag Abilities_Type;
	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_Normal;

	// 能力冷却的标签
	FGameplayTag Abilities_CoolDown_Fire_FireBolt;          // 火球技能冷却时间标签
	FGameplayTag Abilities_CoolDown_Fire_BlazeBurst;        // 焰爆扩散技能冷却时间标签
	FGameplayTag Abilities_CoolDown_Electrical_Electrocute; // 电束技能冷却时间标签
	FGameplayTag Abilities_CoolDown_Arcane_ArcaneShards;    // 奥术碎片冷却时间标签

	// 能力的Block标签
	FGameplayTag Abilities_Block;
	FGameplayTag Abilities_Block_Misc;  // 杂项Block
	FGameplayTag Abilities_Block_Input; // 输入Block
	FGameplayTag Abilities_Block_Input_Pressed;
	FGameplayTag Abilities_Block_Input_Held;
	FGameplayTag Abilities_Block_Input_Released;
	FGameplayTag Abilities_Block_Misc_CursorTrace;

	// 简单的Actor标签
	FName Player = "Player";
	FName Enemy  = "Enemy";


	// 受击反应
	FGameplayTag Abilities_Effects_HitReact_Normal;
	FGameplayTag Abilities_Effects_HitReact_InShock; // 被电击中时

	FGameplayTag GameplayCue_Spawn_ArcaneShards; // 用于生成奥术碎片的GameplayCueTag

	// 用于小部件的标签
	FGameplayTag Widget_Menu_Load_SlotSwitcher_1;
	FGameplayTag Widget_Menu_Load_SlotSwitcher_2;
	FGameplayTag Widget_Menu_Load_SlotSwitcher_3;

	FGameplayTag Widget_Menu_Load_Slot;
	FGameplayTag Widget_Menu_Load_Slot_Enter;
	FGameplayTag Widget_Menu_Load_Slot_Taken;
	FGameplayTag Widget_Menu_Load_Slot_Vacant;

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

	// 伤害类型映射到对应的负面效果的Map
	static TMap<FGameplayTag, FGameplayTag> DamageTypesToDeBuffMap;

	// 包含当前项目所有标签的标签容器
	static FGameplayTagContainer GameplayTagsContainer;          // 包含需要用到的标签(几乎全部)的标签容器
	static FGameplayTagContainer VitalGameplayTagsContainer;     // 和属性集的重要属性一一对应的标签容器
	static FGameplayTagContainer PrimaryGameplayTagsContainer;   // 和属性集的主要属性一一对应的标签容器,容器中的主要属性标签的顺序为SIRV
	static FGameplayTagContainer SecondaryGameplayTagsContainer; // 和属性集的次要属性一一对应的标签容器
	static FGameplayTagContainer DamageTypesTagsContainer;       // 所有伤害类型

	// 包含输入键对应的标签的数组
	static FGameplayTagContainer InputOffensiveTagsContainer;
	static FGameplayTagContainer InputPassiveTagsContainer;

	static FGameplayTagContainer AttackOffensiveTagsContainer;      // 所有主动(能力)技能标签的标签容器
	static FGameplayTagContainer AttackSpellFireTagsContainer;      // 所有火系法术攻击技能的标签容器
	static FGameplayTagContainer AttackSpellLightningTagsContainer; // 所有闪电系法术攻击技能的标签容器
	static FGameplayTagContainer AttackSpellArcaneTagsContainer;    // 所有奥术系法术攻击技能的标签容器
	static FGameplayTagContainer PassiveTagsContainer;              // 所有被动技能所对应的标签容器
	static FGameplayTagContainer DeBuffEffectsTagsContainer;        // 包含所有DeBuff效果的标签容器
private:
	// 存储所有游戏标签的静态类的实例
	static FRPGAuraGameplayTags GameplayTags;
};
