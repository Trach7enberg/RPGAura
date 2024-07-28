// Copyright GGBAO 


#include "CoreTypes/RPGAuraGameplayTags.h"

#include "GameplayTagsManager.h"

FRPGAuraGameplayTags FRPGAuraGameplayTags::GameplayTags;

void FRPGAuraGameplayTags::InitGameplayTags()
{

	GameplayTags.Attribute_Secondary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Attributes.Primary.Strength");
	GameplayTags.Attribute_Secondary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Attributes.Primary.Intelligence");
	GameplayTags.Attribute_Secondary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Attributes.Primary.Resilience");
	GameplayTags.Attribute_Secondary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Attributes.Primary.Vigor");

	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		"Attributes.Secondary.Armor",UTF8TEXT("减少伤害，提高格挡几率"));
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
