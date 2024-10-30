// Copyright GGBAO 


#include "UI/WidgetControllers/SkillTreePassiveWidgetController.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

void USkillTreePassiveWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid()) { return; }

	const auto AbilityInfos = GetWidgetControllerParams().GameInstanceSubsystem->GetAbilityInfoAsset();
	if (!AbilityInfos) { return; }
	
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);

	if (!MyAsc) { return; }
	MyAsc->OnAbilityStatusChanged.AddLambda(
		[this,AbilityInfos,MyAsc](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag,int32 AbilityLevel)
		{
			// 只响应被动技能标签
			if(!AbilityTag.MatchesTag(FRPGAuraGameplayTags::Get().Abilities_Passive)){return;}
			// 广播AbilityInfo用于(被动)技能树技能球按钮的显示状态
			FTagToAbilityInfo AbilityInfo = AbilityInfos->FindPassiveAbilityInfo(AbilityTag);
			AbilityInfo.StatusTag = AbilityStatusTag;
			if (AbilityInfo.InfoDataAbilityIsValid())
			{
				MyAsc->ClientOnSpellButtonAbilityInfoChange(AbilityInfo);
			}
		});
}
void USkillTreePassiveWidgetController::BroadcastInitialValues()
{
	
}
