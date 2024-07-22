// Copyright GGBAO 


#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseAbilitySystemComponentLog, All, All);

void UBaseAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();


}

void UBaseAbilitySystemComponent::InitSetting()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilitySystemComponent::OnGEAppliedToSelf);
}

void UBaseAbilitySystemComponent::OnGEAppliedToSelf(UAbilitySystemComponent *AbilitySystemComponent,
                                                    const FGameplayEffectSpec &GameplayEffectSpec,
                                                    FActiveGameplayEffectHandle ActiveEffectHandle)
{

	FGameplayTagContainer AssetTags;

	// 获取资产标签的容器
	GameplayEffectSpec.GetAllAssetTags(AssetTags);

	if (AssetTags.Num() > 0)
	{
		// 广播资产标签给UI的main控制器
		OnGetAssetTagsDelegate.Broadcast(AssetTags);
	}
}
