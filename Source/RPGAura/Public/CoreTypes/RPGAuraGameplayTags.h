// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGAuraCoreTypes.h"

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

    /// 获取GameplayTags静态实例
    /// @return GameplayTags单例
    static const FRPGAuraGameplayTags& Get()
    {
        return GameplayTags;
    }

    /// 获取所有GameplayTag
    static const TArray<FGameplayTag*>& GetGameplayTagsArray();

    /// 根据属性集属性的名字和标签的类型(全部、重要、主要、次要)获取GameplayTag
    /// @param AttributeName 属性名字
    /// @param TagType 标签类型
    /// @return GameplayTag 
    static FGameplayTag* FindGameplayTagByAttributeName(const FString& AttributeName,
                                                        EGameplayTagType TagType);


    /// 根据游戏标签的类型(全部、重要、主要、次要)返回对应的标签数组
    /// @param TagType 游戏标签的类型(Vital、Primary、Secondary、还是所有标签)
    /// @return 游戏标签数组的指针
    static const TArray<FGameplayTag*>* GetGameplayTagsByType(const EGameplayTagType TagType);

    /// 从存储所有游戏标签的Map中根据属性名字获取对应的游戏标签
    /// @param AttributeName 属性名字
    /// @return 游戏标签
    static FGameplayTag* FindGameplayTagByAttributeName(const FString& AttributeName);

    /// 获取所有游戏标签的Map
    /// @return 存储所有游戏标签的Map
    FORCEINLINE static const TMap<FString, FGameplayTag*>& GetGameplayTagsMap()
    {
        return GameplayTagsMap;
    }

    static TArray<FGameplayTag*> VitalGameplayTagsArray;
    static TArray<FGameplayTag*> PrimaryGameplayTagsArray;
    static TArray<FGameplayTag*> SecondaryGameplayTagsArray;
    static TArray<FGameplayTag*> AllGameplayTagsArray;

private:
    /// 存储所有游戏标签的静态类的实例
    static FRPGAuraGameplayTags GameplayTags;


    static TMap<FString, FGameplayTag*> GameplayTagsMap;

    static FGameplayTag* FindGameplayTagByAttributeName(const FString& AttributeName,
                                                        const TArray<FGameplayTag*>& Array);
};