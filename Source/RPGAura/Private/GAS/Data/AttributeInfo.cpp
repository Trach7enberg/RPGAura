// Copyright GGBAO 


#include "GAS/Data/AttributeInfo.h"

#include "CoreTypes/RPGAuraGameplayTags.h"

DEFINE_LOG_CATEGORY_STATIC(UAttributeInfoLog, All, All);

FRPGAuraAttributeInfo *UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag &AttributeTag)
{

	for (auto &AttributeInfo : RPGAuraAttributeInfos)
	{

		if (AttributeInfo.AttributeTag == AttributeTag)
		{
			return &AttributeInfo;
		}
	}

    UE_LOG(UAttributeInfoLog, Warning, TEXT("找不到 [%s] 对应的属性信息"), *AttributeTag.ToString());
	return nullptr;
}

FRPGAuraAttributeInfo *UAttributeInfo::FindAttributeInfoByAttributeName(const FString &AttributeName)
{
	const auto Tag = FRPGAuraGameplayTags::FindGameplayTagByName(AttributeName);
	if (!Tag)
	{
		UE_LOG(UAttributeInfoLog, Warning, TEXT("找不到对应属性的GT"));
		return nullptr;
	}
	return FindAttributeInfoByTag(*Tag);
}
