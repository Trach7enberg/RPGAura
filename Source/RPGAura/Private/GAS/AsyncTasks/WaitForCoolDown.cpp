// Copyright GGBAO 


#include "GAS/AsyncTasks/WaitForCoolDown.h"

#include "AbilitySystemComponent.h"

UWaitForCoolDown* UWaitForCoolDown::WaitCoolDownChange(UAbilitySystemComponent* AbilitySystemComponent,
                                                       const FGameplayTag& InCoolDownTag)
{
	UWaitForCoolDown* Result = NewObject<UWaitForCoolDown>();
	Result->Asc = AbilitySystemComponent;
	Result->CoolDownTag = InCoolDownTag;

	if (!Result || !IsValid(AbilitySystemComponent) || !InCoolDownTag.IsValid())
	{
		Result->EndTask();
		return nullptr;
	}
	// 绑定当Asc获得CoolDownTag时的回调函数
	Result->Asc->RegisterGameplayTagEvent(Result->CoolDownTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
		Result, &UWaitForCoolDown::OnCoolDownTagChanged);

	// 绑定当Asc每当添加基于持续时间的GE时的回调函数
	Result->OnActiveGameplayEffectDelegateHandle = Result->Asc->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(
		Result, &UWaitForCoolDown::OnActiveGameplayEffectAdded);

	return Result;
}

void UWaitForCoolDown::EndTask()
{
	if (!IsValid(Asc)) { return; }

	// 移除我们绑定的回调函数
	Asc->RegisterGameplayTagEvent(CoolDownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	Asc->OnActiveGameplayEffectAddedDelegateToSelf.Remove(OnActiveGameplayEffectDelegateHandle);
	
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UWaitForCoolDown::OnCoolDownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount)
{
	// 冷却标签被移除,那么技能完成冷却,广播冷却时间为0
	if (NewCount == 0) { CoolDownEnd.Broadcast(0.f); }
}

void UWaitForCoolDown::OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystemComponent,
                                                   const FGameplayEffectSpec& GeSpec,
                                                   FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer AssetTags, GrantedTags;
	GeSpec.GetAllAssetTags(AssetTags);
	GeSpec.GetAllGrantedTags(GrantedTags);

	// GeSpec的资产标签或者被授予的标签容器包含冷却标签时,意味着这个冷却标签所对应的能力在冷却中
	if (AssetTags.HasTagExact(CoolDownTag) || GrantedTags.HasTagExact(CoolDownTag))
	{
		// 创建一个GE查询,查询带有CoolDownTag的GE
		const FGameplayEffectQuery GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			CoolDownTag.GetSingleTagContainer());

		// 获取所有匹配FGameplayEffectQuery这个查询的GamePlayEffect所剩的时间(也就是获取GE蓝图类里的那个Duration的剩余时间)
		// 这个函数将返回所有具有指定标签的游戏效果的剩余时间(但我的冷却标签只会出现在一种类型的GE里不会每个GE都用,因此最多只有一个)
		const auto FloatArray = Asc->GetActiveEffectsTimeRemaining(GameplayEffectQuery);
		if (!FloatArray.Num()) { return; }

		// 防止我们同时有多个带有冷却标签的GE的时间,我们找出最大值,这个最大值就是我们最近所触发的冷却
		float MaxTimeRemaining = FloatArray[0];
		for (int i = 1; i < FloatArray.Num(); ++i)
		{
			if (FloatArray[i] > MaxTimeRemaining) { MaxTimeRemaining = FloatArray[i]; }
		}

		CoolDownStart.Broadcast(MaxTimeRemaining);
	}
}
