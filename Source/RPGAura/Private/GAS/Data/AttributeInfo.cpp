// Copyright GGBAO 


#include "GAS/Data/AttributeInfo.h"

DEFINE_LOG_CATEGORY_STATIC(UAttributeInfoLog, All, All);

FRPGAuraAttributeInfo UAttributeInfo::GetAttributeInfoByTag(const FGameplayTag &AttributeTag,
                                                            const bool BLogNotFound) const
{

	for (const auto AttributeInfo : RPGAuraAttributeInfos)
	{
		if (AttributeInfo.AttributeTag == AttributeTag)
		{
			return AttributeInfo;
		}
	}

	if (BLogNotFound)
	{
		UE_LOG(UAttributeInfoLog, Warning, TEXT("找不到 [%s] 对应的属性信息"), *AttributeTag.ToString());
	}
	return FRPGAuraAttributeInfo{};
}
