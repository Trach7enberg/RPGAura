// Copyright GGBAO 


#include "CoreTypes/RPGAuraGameplayTags.h"

#include "GameplayTagsManager.h"

DEFINE_LOG_CATEGORY_STATIC(FRPGAuraGameplayTagsLog, All, All);

FRPGAuraGameplayTags FRPGAuraGameplayTags::GameplayTags;
TArray<FGameplayTag*> FRPGAuraGameplayTags::VitalGameplayTagsArray;
TArray<FGameplayTag*> FRPGAuraGameplayTags::PrimaryGameplayTagsArray;
TArray<FGameplayTag*> FRPGAuraGameplayTags::SecondaryGameplayTagsArray;
TArray<FGameplayTag*> FRPGAuraGameplayTags::AllGameplayTagsArray;;
TMap<FString, FGameplayTag*> FRPGAuraGameplayTags::GameplayTagsMap;

void FRPGAuraGameplayTags::InitGameplayTags()
{
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

    // 添加到Vital数组
    {
        GameplayTags.VitalGameplayTagsArray.Add(&GameplayTags.Attribute_Vital_CurrentHealth);
        GameplayTags.VitalGameplayTagsArray.Add(&GameplayTags.Attribute_Vital_CurrentMana);
    }

    // 添加到Primary数组
    {
        GameplayTags.PrimaryGameplayTagsArray.Add(&(GameplayTags.Attribute_Primary_Strength));
        GameplayTags.PrimaryGameplayTagsArray.Add(&GameplayTags.Attribute_Primary_Intelligence);
        GameplayTags.PrimaryGameplayTagsArray.Add(&GameplayTags.Attribute_Primary_Resilience);
        GameplayTags.PrimaryGameplayTagsArray.Add(&GameplayTags.Attribute_Primary_Vigor);
    }

    // 添加到Secondary数组
    {
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_Armor);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_ArmorPenetration);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_BlockChance);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_CriticalHitChance);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_CriticalHitResistance);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_CriticalHitDamage);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_HealthRegeneration);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_ManaRegeneration);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_MaxHealth);
        GameplayTags.SecondaryGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_MaxMana);
    }

    // 添加标签到all数组
    {
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Vital_CurrentHealth);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Vital_CurrentMana);

        GameplayTags.AllGameplayTagsArray.Add(&(GameplayTags.Attribute_Primary_Strength));
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Primary_Intelligence);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Primary_Resilience);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Primary_Vigor);

        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_Armor);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_ArmorPenetration);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_BlockChance);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_CriticalHitChance);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_CriticalHitResistance);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_CriticalHitDamage);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_HealthRegeneration);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_ManaRegeneration);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_MaxHealth);
        GameplayTags.AllGameplayTagsArray.Add(&GameplayTags.Attribute_Secondary_MaxMana);
    }

    //
    {
        GameplayTags.GameplayTagsMap.Add("CurrentHealth", &GameplayTags.Attribute_Vital_CurrentHealth);
        GameplayTags.GameplayTagsMap.Add("CurrentMana", &GameplayTags.Attribute_Vital_CurrentMana);

        GameplayTags.GameplayTagsMap.Add("Strength", &(GameplayTags.Attribute_Primary_Strength));
        GameplayTags.GameplayTagsMap.Add("Intelligence", &GameplayTags.Attribute_Primary_Intelligence);
        GameplayTags.GameplayTagsMap.Add("Resilience", &GameplayTags.Attribute_Primary_Resilience);
        GameplayTags.GameplayTagsMap.Add("Vigor", &GameplayTags.Attribute_Primary_Vigor);

        GameplayTags.GameplayTagsMap.Add("Armor", &GameplayTags.Attribute_Secondary_Armor);
        GameplayTags.GameplayTagsMap.Add("ArmorPenetration", &GameplayTags.Attribute_Secondary_ArmorPenetration);
        GameplayTags.GameplayTagsMap.Add("BlockChance", &GameplayTags.Attribute_Secondary_BlockChance);
        GameplayTags.GameplayTagsMap.Add("CriticalHitChance", &GameplayTags.Attribute_Secondary_CriticalHitChance);
        GameplayTags.GameplayTagsMap.Add("CriticalHitResistance", &GameplayTags.Attribute_Secondary_CriticalHitResistance);
        GameplayTags.GameplayTagsMap.Add("CriticalHitDamage", &GameplayTags.Attribute_Secondary_CriticalHitDamage);
        GameplayTags.GameplayTagsMap.Add("HealthRegeneration", &GameplayTags.Attribute_Secondary_HealthRegeneration);
        GameplayTags.GameplayTagsMap.Add("ManaRegeneration", &GameplayTags.Attribute_Secondary_ManaRegeneration);
        GameplayTags.GameplayTagsMap.Add("MaxHealth", &GameplayTags.Attribute_Secondary_MaxHealth);
        GameplayTags.GameplayTagsMap.Add("MaxMana", &GameplayTags.Attribute_Secondary_MaxMana);
    }
}

const TArray<FGameplayTag*>& FRPGAuraGameplayTags::GetGameplayTagsArray()
{
    return AllGameplayTagsArray;
}

FGameplayTag* FRPGAuraGameplayTags::FindGameplayTagByAttributeName(const FString& AttributeName,
                                                                   const EGameplayTagType TagType)
{
    switch (TagType)
    {
        case EGameplayTagType::AllGameplayTags:
            return FindGameplayTagByAttributeName(AttributeName, AllGameplayTagsArray);
        case EGameplayTagType::VitalGameplayTags:
            return FindGameplayTagByAttributeName(AttributeName, VitalGameplayTagsArray);

        case EGameplayTagType::PrimaryGameplayTags:
            return FindGameplayTagByAttributeName(AttributeName, PrimaryGameplayTagsArray);

        case EGameplayTagType::SecondaryGameplayTags:
            return FindGameplayTagByAttributeName(AttributeName, SecondaryGameplayTagsArray);

        default:
            return nullptr;
    }
}

const TArray<FGameplayTag*>* FRPGAuraGameplayTags::GetGameplayTagsByType(const EGameplayTagType TagType)
{
    switch (TagType)
    {
        case EGameplayTagType::AllGameplayTags:
            return &AllGameplayTagsArray;
            break;
        case EGameplayTagType::VitalGameplayTags:
            return &VitalGameplayTagsArray;
            break;
        case EGameplayTagType::PrimaryGameplayTags:
            return &PrimaryGameplayTagsArray;
            break;
        case EGameplayTagType::SecondaryGameplayTags:
            return &SecondaryGameplayTagsArray;
            break;
    }

    return nullptr;
}

FGameplayTag* FRPGAuraGameplayTags::FindGameplayTagByAttributeName(const FString& AttributeName)
{
    const auto Result = GameplayTagsMap.Find(AttributeName);
    return Result ? *Result : nullptr;
}

FGameplayTag* FRPGAuraGameplayTags::FindGameplayTagByAttributeName(const FString& AttributeName,
                                                                   const TArray<FGameplayTag*>& Array)
{
    for (const auto GameplayTag : Array)
    {
        if (GameplayTag->ToString().Contains(AttributeName))
        {
            return GameplayTag;
        }
    }
    return nullptr;
}