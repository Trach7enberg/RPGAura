// Copyright GGBAO 


#include "GAS/Data/AttributeInfo.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
DEFINE_LOG_CATEGORY_STATIC(UAttributeInfoLog, All, All);

UAttributeInfo::UAttributeInfo()
{
    for (const auto& GameplayTag : FRPGAuraGameplayTags::GameplayTagsContainer)
    {
        FRPGAuraAttributeInfo Tmp;
        Tmp.AttributeTag = GameplayTag;
        RPGAuraAttributeInfos.Add(Tmp);
    }
}

FRPGAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag)
{
    for (const auto& AttributeInfo : RPGAuraAttributeInfos)
    {
        if (AttributeInfo.AttributeTag.MatchesTag(AttributeTag))
        {
            return AttributeInfo;
        }
    }

    UE_LOG(UAttributeInfoLog, Warning, TEXT("找不到 [%s] 对应的属性信息"), *AttributeTag.ToString());
    return FRPGAuraAttributeInfo{};
}