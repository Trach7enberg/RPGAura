// Copyright GGBAO 


#include "UI/WidgetControllers/SkillTreePassiveWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

void USkillTreePassiveWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid()) { return; }

	const auto AbilityInfos = URPGAuraGameInstanceSubsystem::GetAbilityInfoAsset(
		GetWidgetControllerParams().CurrentPlayerController);
	if (!AbilityInfos) { return; }
	
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);

	if (!MyAsc) { return; }
	MyAsc->OnAbilityStatusChanged.AddLambda(
		[this,AbilityInfos](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag,int32 AbilityLevel)
		{
			// 广播AbilityInfo用于(被动)技能树技能球按钮的显示状态
			FTagToAbilityInfo AbilityInfo = AbilityInfos->FindOffensiveAbilityInfo(AbilityTag);
			AbilityInfo.StatusTag = AbilityStatusTag;
			if (AbilityInfo.InfoDataAbilityIsValid())
			{
				GetWidgetControllerParams().GameInstanceSubsystem->OnSpellButtonAbilityInfoChange.Broadcast(AbilityInfo);
			}
		});
}
void USkillTreePassiveWidgetController::BroadcastInitialValues()
{
	
}
