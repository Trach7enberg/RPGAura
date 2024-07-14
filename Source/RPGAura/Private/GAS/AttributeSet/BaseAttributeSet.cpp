// Copyright GGBAO 


#include "GAS/AttributeSet/BaseAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UBaseAttributeSet::UBaseAttributeSet() {}

void UBaseAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData &OldHealth) const
{
	// 通知GAS currentHealth刚刚被复制了,使GAS整个系统协调工作,让能力系统记录这一变化并保存旧值的跟踪,以防止需要回滚
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CurrentHealth, OldHealth);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData &OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_CurrentMana(const FGameplayAttributeData &OldCurrentMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CurrentMana, OldCurrentMana);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData &OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 注册health变量用于复制,这是复制任何变量所需的步骤
	// 复制条件 COND_None ,也就是没有条件的复制
	// REPNOTIFY_Always 意味着如果在服务器上设置了值,就复制给客户端,那个值将被更新和设置,这里的值就是我们的CurrentHealth
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CurrentMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}
