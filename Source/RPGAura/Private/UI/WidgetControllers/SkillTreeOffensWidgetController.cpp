// Copyright GGBAO 


#include "UI/WidgetControllers/SkillTreeOffensWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(USkillTreeOffensWidgetControllerLog, All, All);

void USkillTreeOffensWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid()) { return; }

	const auto MyGi = GetWidgetControllerParams().CurrentPlayerController->GetGameInstance()->GetSubsystem<
		URPGAuraGameInstanceSubsystem>();

	const auto AbilityInfos = URPGAuraGameInstanceSubsystem::GetAbilityInfoAsset(
		GetWidgetControllerParams().CurrentPlayerController);
	if (!AbilityInfos) { return; }
	
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);

	if (!MyAsc) { return; }
	MyAsc->OnAbilityStatusChanged.AddLambda(
		[this,AbilityInfos](const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag)
		{
			FTagToAbilityInfo AbilityInfo = AbilityInfos->FindOffensiveAbilityInfo(AbilityTag);
			AbilityInfo.StatusTag = AbilityStatusTag;
			if (AbilityInfo.InfoDataAbilityIsValid()) { GetWidgetControllerParams().GameInstanceSubsystem->AbilityInfoDelegate.Broadcast(AbilityInfo); }
		});
}

void USkillTreeOffensWidgetController::BroadcastInitialValues() {}
