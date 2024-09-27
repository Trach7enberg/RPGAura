// Copyright GGBAO 


#include "UI/WidgetControllers/TextValueWidgetController.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "Interfaces/PlayerInterface.h"
#include "PlayerStates/BasePlayerState.h"


DEFINE_LOG_CATEGORY_STATIC(UTextValueWidgetControllerLog, All, All);

void UTextValueWidgetController::BindCallBack()
{
	if (!IsWidgetControllerParamsValid()) { return; }

	const auto MyPs = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);
	if (!MyPs) { return; }
	
	MyPs->AssignableAttributePointsChangeDelegate.AddLambda([this](int32 NewValue)
	{
		OnAssignableAttributePointsChange.Broadcast(NewValue);
	});

	MyPs->AssignableSpellPointsChangeDelegate.AddLambda([this](int32 NewValue)
	{
		OnAssignableSpellPointsChange.Broadcast(NewValue);
	});
}

void UTextValueWidgetController::BroadcastInitialValues()
{
	if (!IsWidgetControllerParamsValid()) { return; }
	const auto MyPs = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);
	if (MyPs)
	{
		OnAssignableAttributePointsChange.Broadcast(MyPs->GetAssignableAttributePoints());
		OnAssignableSpellPointsChange.Broadcast(MyPs->GetAssignableSpellPoints());
	}
}

void UTextValueWidgetController::CostPoint(const FGameplayTag& AttributeTag) { UpgradeAttribute(AttributeTag); }

void UTextValueWidgetController::SpendSpellPoint(const FGameplayTag& AbilityTag)
{
	if (!IsWidgetControllerParamsValid() || !AbilityTag.IsValid()) { return ; }

	const auto MyAsc =
		Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);

	if (!MyAsc) { return ; }
	MyAsc->UpgradeSpellPoint(AbilityTag);
}

bool UTextValueWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (!IsWidgetControllerParamsValid()) { return false; }

	const auto MyAsc =
		Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);

	if (!MyAsc) { return false; }

	MyAsc->UpgradeAttribute(AttributeTag);
	return true;
}
