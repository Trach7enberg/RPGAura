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

void FRPGAuraGameplayTags::InitGameplayTags()
{
	// 创建Native GameplayTags
	{
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
			GameplayTags.Abilities_Damage_Spell_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Attributes.Secondary.Resistance.Arcane");
		}

		// 输入动作标签
		{
			GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.LMB", FString("左键"));
			GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.RMB", FString("右键"));
			GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.1", FString("主键盘1"));
			GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.2", FString("主键盘2"));
			GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.3", FString("主键盘3"));
			GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag("InputTag.4", FString("主键盘4"));
		}

		// 蒙太奇事件标签
		{
			GameplayTags.Event_Montage_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Event.Montage.FireBolt", FString("识别GameplayEvent的标签,用于当播放火箭蒙太奇触发某个通知时发送gameplay事件"));
		}

		// 伤害类型标签
		{
			GameplayTags.Abilities_Damage_Spell = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Damage.Spell", FString("法术节点父类"));
			GameplayTags.Abilities_Damage_Spell_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Damage.Spell.Physical", FString("物理法术伤害"));
			GameplayTags.Abilities_Damage_Spell_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Damage.Spell.Fire", FString("火焰法术伤害"));
			GameplayTags.Abilities_Damage_Spell_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Damage.Spell.Lightning", FString("闪电法术伤害"));
			GameplayTags.Abilities_Damage_Spell_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Damage.Spell.Arcane", FString("奥术法术伤害"));

			GameplayTags.Abilities_Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
				"Abilities.Effects.HitReact", FString("被击中时给予的标签"));
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
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Physical);
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Fire);
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Lightning);
		GameplayTags.DamageTypesTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Arcane);
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

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_LMB);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_RMB);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_1);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_2);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_3);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.InputTag_4);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Physical);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Fire);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Lightning);
		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Abilities_Damage_Spell_Arcane);

		GameplayTags.GameplayTagsContainer.AddTag(GameplayTags.Event_Montage_FireBolt);
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
		
		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Physical, EGameplayTagNum::PhysicalResistance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Fire, EGameplayTagNum::FireResistance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Lightning, EGameplayTagNum::LightningResistance);
		GameplayTags.TagToNumMap.Add(GameplayTags.Attributes_Secondary_Resistance_Arcane, EGameplayTagNum::ArcaneResistance);
		

		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_LMB, EGameplayTagNum::InputLMB);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_RMB, EGameplayTagNum::InputRMB);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_1, EGameplayTagNum::Input1);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_2, EGameplayTagNum::Input2);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_3, EGameplayTagNum::Input3);
		GameplayTags.TagToNumMap.Add(GameplayTags.InputTag_4, EGameplayTagNum::Input4);


		GameplayTags.TagToNumMap.Add(GameplayTags.Event_Montage_FireBolt, EGameplayTagNum::Event_Montage_FireBolt);

		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_Damage_Spell_Physical,
		                             EGameplayTagNum::Abilities_Damage_Spell_Physical);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_Damage_Spell_Fire,
		                             EGameplayTagNum::Abilities_Damage_Spell_Fire);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_Damage_Spell_Lightning,
		                             EGameplayTagNum::Abilities_Damage_Spell_Lightning);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_Damage_Spell_Arcane,
		                             EGameplayTagNum::Abilities_Damage_Spell_Arcane);
		GameplayTags.TagToNumMap.Add(GameplayTags.Abilities_Effects_HitReact,
		                             EGameplayTagNum::Abilities_Effects_HitReact);
	}

	// 伤害类型到对应的抵抗类型映射
	{
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_Damage_Spell_Fire,
		                                             GameplayTags.Attributes_Secondary_Resistance_Fire);
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_Damage_Spell_Lightning,
		                                             GameplayTags.Attributes_Secondary_Resistance_Lightning);
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_Damage_Spell_Arcane,
		                                             GameplayTags.Attributes_Secondary_Resistance_Arcane);
		GameplayTags.DamageTypesToResistancesMap.Add(GameplayTags.Abilities_Damage_Spell_Physical,
		                                             GameplayTags.Attributes_Secondary_Resistance_Physical);
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
