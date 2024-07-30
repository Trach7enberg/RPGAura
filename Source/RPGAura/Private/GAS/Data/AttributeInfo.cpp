// Copyright GGBAO 


#include "GAS/Data/AttributeInfo.h"

#include "CoreTypes/RPGAuraGameplayTags.h"

DEFINE_LOG_CATEGORY_STATIC(UAttributeInfoLog, All, All);

FRPGAuraAttributeInfo *UAttributeInfo::GetAttributeInfoByTag(const FGameplayTag &AttributeTag,
                                                             const bool BLogNotFound)
{

	for (auto &AttributeInfo : RPGAuraAttributeInfos)
	{

		if (AttributeInfo.AttributeTag == AttributeTag)
		{
			return &AttributeInfo;
		}
	}

	if (BLogNotFound)
	{
		UE_LOG(UAttributeInfoLog, Warning, TEXT("找不到 [%s] 对应的属性信息"), *AttributeTag.ToString());
	}
	return nullptr;
}

FRPGAuraAttributeInfo *UAttributeInfo::GetAttributeInfoByAttributeName(const FString &AttributeName, bool BLogNotFound)
{
	const auto Tag = FRPGAuraGameplayTags::GetGameplayTagByAttributeName(AttributeName);
	if (!Tag)
	{
		UE_LOG(UAttributeInfoLog, Warning, TEXT("找不到对应属性的GT"));
		return nullptr;
	}
	return GetAttributeInfoByTag(*Tag);
}
