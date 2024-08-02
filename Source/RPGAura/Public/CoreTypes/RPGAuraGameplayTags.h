// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RPGAuraCoreTypes.h"

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

    

    /// 获取所有游戏标签的Map
    /// @return 存储所有游戏标签的Map
    FORCEINLINE static const TMap<FString, FGameplayTag*>& GetGameplayTagsMap()
    {
        return GameplayTagsMap;
    }

    /// 从存储所有游戏标签的Map中根据属性名字获取对应的游戏标签
    /// @param Name 属性名字
    /// @return 游戏标签
    static FGameplayTag* FindGameplayTagByName(const FString& Name);
    
    static  EGameplayTagNum* FindEnumByTag(const FGameplayTag& GameplayTag);

    static TArray<FGameplayTag*> VitalGameplayTagsArray;
    static TArray<FGameplayTag*> PrimaryGameplayTagsArray;
    static TArray<FGameplayTag*> SecondaryGameplayTagsArray;
    static TArray<FGameplayTag*> AllGameplayTagsArray;
    static TMap<FGameplayTag, EGameplayTagNum> TagToNumMap;

private:
    /// 存储所有游戏标签的静态类的实例
    static FRPGAuraGameplayTags GameplayTags;


    static TMap<FString, FGameplayTag*> GameplayTagsMap;

    /// 根据名字获取游戏标签
    /// @param Name 名字
    /// @param Array 数组
    /// @return 游戏标签
    static FGameplayTag* FindGameplayTagByName(const FString& Name,
                                               const TArray<FGameplayTag*>& Array);
};