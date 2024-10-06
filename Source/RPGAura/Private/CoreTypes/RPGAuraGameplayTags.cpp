// Copyright GGBAO 


#include "CoreTypes/RPGAuraGameplayTags.h"

#include "GameplayTagsManager.h"

DEFINE_LOG_CATEGORY_STATIC(FRPGAuraGameplayTagsLog, All, All);

FRPGAuraGameplayTags FRPGAuraGameplayTags::GameplayTags;

TMap<FGameplayTag, EGameplayTagNum> FRPGAuraGameplayTags::TagToNumMap;
TMap<FGameplayTag, FGameplayTag> FRPGAuraGameplayTags::DamageTypesToResistancesMap;

FGameplayTagContainer FRPGAuraGameplayTags::GameplayTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::VitalGameplayTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::PrimaryGameplayTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::SecondaryGameplayTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::DamageTypesTagsContainer;

FGameplayTagContainer FRPGAuraGameplayTags::InputOffensiveTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::InputPassiveTagsContainer;

FGameplayTagContainer FRPGAuraGameplayTags::AttackOffensiveTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::AttackSpellFireTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::AttackSpellLightningTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::AttackSpellArcaneTagsContainer;
FGameplayTagContainer FRPGAuraGameplayTags::PassiveTagsContainer;

void FRPGAuraGameplayTags::InitGameplayTags()
{
	// 创建Native GameplayTags
	{
		GameplayTags.Attribute_Main = UGameplayTagsManager::Get().AddNativeGameplayTag(
			"Attributes.Main");
		// 主要属性
		{
			GameplayTags.Attribute_Vital_CurrentHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Vital.CurrentHealth");
			GameplayTags.Attribute_Vital_CurrentMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Vital.CurrentMana");

			GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Primary.Strength");
			GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Primary.Intelligence");
			GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Primary.Resilience");
			GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Primary.Vigor");
		}

		// 次要属性
		{
			GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.Armor",
				UTF8TEXT("减少伤害，提高格挡几率"));
			GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.ArmorPenetration");
			GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.BlockChance");
			GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.CriticalHitChance");
			GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.CriticalHitResistance");
			GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.CriticalHitDamage");
			GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.HealthRegeneration");
			GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.ManaRegeneration");
			GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.MaxHealth");
			GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.MaxMana");
		}

		// 次要抵抗属性
		{
			GameplayTags.Attributes_Secondary_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.Resistance.Fire");
			GameplayTags.Attributes_Secondary_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.Resistance.Physical");
			GameplayTags.Attributes_Secondary_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.Resistance.Lightning");
			GameplayTags.Attributes_Secondary_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.Resistance.Arcane");
		}

		// 元属性
		{
			GameplayTags.Attributes_Meta_InComingDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Meta.InComingDamage");

			GameplayTags.Attributes_Meta_InComingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Meta.InComingXP");
		}

		// 输入动作标签
		{
			GameplayTags.InputTag = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag", FString(""));
			GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.LMB", FString("左键"));
			GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.RMB", FString("右键"));
			GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.1", FString("主键盘1"));
			GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.2", FString("主键盘2"));
			GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.3", FString("主键盘3"));
			GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.4", FString("主键盘4"));
			GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"InputTag.Passive1", FString(""));
			GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"InputTag.Passive2", FString(""));
		}


		// 伤害类型标签
		{
			GameplayTags.Abilities_DamageType_Spell = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.DamageType.Spell", FString("法术伤害节点父类"));
			GameplayTags.Abilities_DamageType_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.DamageType.Physical", FString("物理法术伤害"));
			GameplayTags.Abilities_DamageType_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.DamageType.Fire", FString("火焰法术伤害"));
			GameplayTags.Abilities_DamageType_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.DamageType.Lightning", FString("闪电法术伤害"));
			GameplayTags.Abilities_DamageType_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.DamageType.Arcane", FString("奥术法术伤害"));

			GameplayTags.Abilities_Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Effects.HitReact", FString("被击中时给予的标签"));
		}

		// 蒙太奇事件标签
		{
			GameplayTags.CombatSocket_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Montage.CombatSocket.Normal", FString(""));
			GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Montage.CombatSocket.LeftHand", FString(""));
			GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Montage.CombatSocket.RightHand", FString(""));
		}

		// 主动攻击能力的标签
		{
			GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Attack", FString());
			GameplayTags.Abilities_Attack_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Attack.Melee", FString("启动近战攻击能力的标签"));
			GameplayTags.Abilities_Attack_Range = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Attack.Range", FString("启动远程攻击能力的标签"));
			GameplayTags.Abilities_Attack_Spell = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Attack.Spell", FString("启动萨满法术攻击能力的标签"));
			GameplayTags.Abilities_Attack_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Attack.Summon", FString("召唤法术能力"));

			GameplayTags.Abilities_Attack_Spell_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Attack.Spell.Fire.FireBolt", FString("火球法术技能"));
			GameplayTags.Abilities_Attack_Spell_Lightning_Electrocute = UGameplayTagsManager::Get().
				AddNativeGameplayTag(
					"Abilities.Attack.Spell.Lightning.Electrocute", FString("触电法术技能"));
		}

		// 被动能力标识
		{
			GameplayTags.Abilities_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Passive", FString());
			GameplayTags.Abilities_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Passive.1", FString());
			GameplayTags.Abilities_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Passive.2", FString());
		}

		//能力状态
		{
			GameplayTags.Abilities_Status = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Status", FString(""));
			GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Status.Locked", FString(""));
			GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Status.Unlocked", FString(""));
			GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Status.Eligible", FString(""));
			GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Status.Equipped", FString(""));
		}

		//能力的分类
		{
			GameplayTags.Abilities_Type = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Type", FString(""));
			GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Type.Offensive", FString(""));
			GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Type.Passive", FString(""));
			GameplayTags.Abilities_Type_Normal = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Type.Normal", FString(""));
		}

		// 技能冷却标签
		{
			GameplayTags.Abilities_CoolDown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.CoolDown.Fire.FireBolt", FString(""));
		}
	}

	// 添加到Vital 标签容器
	{
		GameplayTags.VitalGameplayTagsContainer.AddTag(GameplayTags.Attribute_Vital_CurrentHealth);
		GameplayTags.VitalGameplayTagsContainer.AddTag(GameplayTags.Attribute_Vital_CurrentMana);
	}

	// 添加到Primary 标签容器
	{
		GameplayTags.PrimaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Primary_Strength);
		GameplayTags.PrimaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Primary_Intelligence);
		GameplayTags.PrimaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Primary_Resilience);
		GameplayTags.PrimaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Primary_Vigor);
	}

	// 添加到Secondary 标签容器
	{
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_Armor);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_ArmorPenetration);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_BlockChance);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_CriticalHitChance);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_CriticalHitResistance);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_CriticalHitDamage);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_HealthRegeneration);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_ManaRegeneration);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_MaxHealth);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_MaxMana);

		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Physical);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Fire);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Lightning);
		GameplayTags.SecondaryGameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Arcane);
	}

	// 添加所有伤害类型标签到对应的TagContainer
	{
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Physical);
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Fire);
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Lightning);
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Arcane);
	}

	// 添加所有标签到TagContainer
	{
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Vital_CurrentHealth);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Vital_CurrentMana);

		GameplayTags.GameplayTagsContainer.AddTag((GameplayTags.Attribute_Primary_Strength));
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Primary_Intelligence);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Primary_Resilience);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Primary_Vigor);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_Armor);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_ArmorPenetration);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_BlockChance);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_CriticalHitChance);
		GameplayTags.GameplayTagsContainer.AddTag(
			GameplayTags.Attribute_Secondary_CriticalHitResistance);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_CriticalHitDamage);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_HealthRegeneration);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_ManaRegeneration);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_MaxHealth);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attribute_Secondary_MaxMana);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Physical);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Fire);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Lightning);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Attributes_Secondary_Resistance_Arcane);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_LMB);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_RMB);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_1);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_2);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_3);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_4);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_Passive_1);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_Passive_2);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Physical);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Fire);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Lightning);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_DamageType_Arcane);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.CombatSocket_Normal);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.CombatSocket_LeftHand);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.CombatSocket_RightHand);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Attack);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Attack_Melee);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Attack_Range);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Attack_Spell);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Attack_Summon);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Attack_Spell_Fire_FireBolt);
	}

	// 标签到枚举的映射
	{
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Vital_CurrentHealth, EGameplayTagNum::CurrentHealth);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Vital_CurrentMana, EGameplayTagNum::CurrentMana);

		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Primary_Strength, EGameplayTagNum::Strength);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Primary_Intelligence, EGameplayTagNum::Intelligence);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Primary_Resilience, EGameplayTagNum::Resilience);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Primary_Vigor, EGameplayTagNum::Vigor);

		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_Armor, EGameplayTagNum::Armor);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_ArmorPenetration,
		                             EGameplayTagNum::ArmorPenetration);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_BlockChance, EGameplayTagNum::BlockChance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_CriticalHitChance,
		                             EGameplayTagNum::CriticalHitChance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance,
		                             EGameplayTagNum::CriticalHitResistance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage,
		                             EGameplayTagNum::CriticalHitDamage);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_HealthRegeneration,
		                             EGameplayTagNum::HealthRegeneration);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_ManaRegeneration,
		                             EGameplayTagNum::ManaRegeneration);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_MaxHealth, EGameplayTagNum::MaxHealth);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attribute_Secondary_MaxMana, EGameplayTagNum::MaxMana);

		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Physical,
		                             EGameplayTagNum::PhysicalResistance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Fire,
		                             EGameplayTagNum::FireResistance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Lightning,
		                             EGameplayTagNum::LightningResistance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Arcane,
		                             EGameplayTagNum::ArcaneResistance);


		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_LMB, EGameplayTagNum::InputLMB);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_RMB, EGameplayTagNum::InputRMB);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_1, EGameplayTagNum::Input1);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_2, EGameplayTagNum::Input2);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_3, EGameplayTagNum::Input3);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_4, EGameplayTagNum::Input4);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_Passive_1, EGameplayTagNum::Input_Passive1);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_Passive_2, EGameplayTagNum::Input_Passive2);

		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_DamageType_Physical,
		                             EGameplayTagNum::Abilities_Damage_Spell_Physical);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_DamageType_Fire,
		                             EGameplayTagNum::Abilities_Damage_Spell_Fire);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_DamageType_Lightning,
		                             EGameplayTagNum::Abilities_Damage_Spell_Lightning);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_DamageType_Arcane,
		                             EGameplayTagNum::Abilities_Damage_Spell_Arcane);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_Effects_HitReact,
		                             EGameplayTagNum::Abilities_Effects_HitReact);

		GameplayTags.TagToNumMap.Add(GameplayTags.CombatSocket_Normal,
		                             EGameplayTagNum::Montage_Attack_Normal);
		GameplayTags.TagToNumMap.Add(GameplayTags.CombatSocket_Normal,
		                             EGameplayTagNum::Montage_Attack_LeftHand);
		GameplayTags.TagToNumMap.Add(GameplayTags.CombatSocket_Normal,
		                             EGameplayTagNum::Montage_Attack_RightHand);
	}

	// 伤害类型到对应的抵抗类型映射
	{
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_DamageType_Fire,
		                                             GameplayTags.Attributes_Secondary_Resistance_Fire);
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_DamageType_Lightning,
		                                             GameplayTags.Attributes_Secondary_Resistance_Lightning);
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_DamageType_Arcane,
		                                             GameplayTags.Attributes_Secondary_Resistance_Arcane);
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_DamageType_Physical,
		                                             GameplayTags.Attributes_Secondary_Resistance_Physical);
	}

	// 主动输入技能的输入键添加到对应容器
	{
		GameplayTags.InputOffensiveTagsContainer.AddTag(GameplayTags.InputTag_LMB);
		GameplayTags.InputOffensiveTagsContainer.AddTag(GameplayTags.InputTag_RMB);
		GameplayTags.InputOffensiveTagsContainer.AddTag(GameplayTags.InputTag_1);
		GameplayTags.InputOffensiveTagsContainer.AddTag(GameplayTags.InputTag_2);
		GameplayTags.InputOffensiveTagsContainer.AddTag(GameplayTags.InputTag_3);
		GameplayTags.InputOffensiveTagsContainer.AddTag(GameplayTags.InputTag_4);
	}
	
	// 添加被动标识标签添加到对应容器
	{
		GameplayTags.InputPassiveTagsContainer.AddTag(GameplayTags.InputTag_Passive_1);
		GameplayTags.InputPassiveTagsContainer.AddTag(GameplayTags.InputTag_Passive_2);
	}
	// 添加火系法术攻击技能标签到容器里
	{
		GameplayTags.AttackSpellFireTagsContainer.AddTag(GameplayTags.Abilities_Attack_Spell_Fire_FireBolt);
	}

	// 添加电系技能标签到容器里
	{
		GameplayTags.AttackSpellLightningTagsContainer.
		             AddTag(GameplayTags.Abilities_Attack_Spell_Lightning_Electrocute);
	}

	// 添加所有主动攻击标签到容器
	{
		GameplayTags.AttackOffensiveTagsContainer.AppendTags(GameplayTags.AttackSpellFireTagsContainer);
		GameplayTags.AttackOffensiveTagsContainer.AppendTags(GameplayTags.AttackSpellFireTagsContainer);
		GameplayTags.AttackOffensiveTagsContainer.AppendTags(GameplayTags.AttackSpellArcaneTagsContainer);
	}

	// 添加被动技能标签到对应的容器
	{
		GameplayTags.PassiveTagsContainer.AddTag(GameplayTags.Abilities_Passive_1);
		GameplayTags.PassiveTagsContainer.AddTag(GameplayTags.Abilities_Passive_2);
	}
}

const FGameplayTagContainer& FRPGAuraGameplayTags::GetTagsContainerByType(const EGameplayTagType TagType)
{
	switch (TagType)
	{
	case EGameplayTagType::VitalGameplayTags:
		return VitalGameplayTagsContainer;
	case EGameplayTagType::PrimaryGameplayTags:
		return PrimaryGameplayTagsContainer;
	case EGameplayTagType::SecondaryGameplayTags:
		return SecondaryGameplayTagsContainer;
	default: break;
	}

	return FGameplayTagContainer::EmptyContainer;
}


EGameplayTagNum* FRPGAuraGameplayTags::FindEnumByTag(const FGameplayTag& GameplayTag)
{
	const auto Result = TagToNumMap.Find(GameplayTag);
	return Result ? Result : nullptr;
}
