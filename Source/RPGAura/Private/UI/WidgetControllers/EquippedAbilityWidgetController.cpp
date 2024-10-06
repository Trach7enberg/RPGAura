// Copyright GGBAO 


#include "UI/WidgetControllers/EquippedAbilityWidgetController.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(UEquippedAbilityWidgetControllerLog, All, All);

void UEquippedAbilityWidgetController::BindCallBack()
{
	
}
void UEquippedAbilityWidgetController::BroadcastInitialValues()
{
	if (!IsWidgetControllerParamsValid())
	{
		UE_LOG(UEquippedAbilityWidgetControllerLog, Error, TEXT("[%s] , [%hs]Widget控制器参数无效"), *GetNameSafe(this),
			   __FUNCTION__);
		return;
	}

	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if(!MyAsc){return;}

	MyAsc->BroadCastDefaultActivatableAbilitiesInfo();
}

void UEquippedAbilityWidgetController::EquipAbility(const FGameplayTag& SelectedAbilityTag,
											  const FGameplayTag& SelectedInputSlotTag)
{
	if (!IsWidgetControllerParamsValid() || !SelectedAbilityTag.IsValid() || !SelectedInputSlotTag.IsValid()) { return; }
	const auto MyAsc = Cast<UBaseAbilitySystemComponent>(GetWidgetControllerParams().CurrentAbilitySystemComponent);
	if (!MyAsc) { return; }

	const auto Spec = MyAsc->GetSpecFromAbilityTag(SelectedAbilityTag);
	if (!Spec) { return; }


	const auto AbilityStatus = MyAsc->GetAbilityStatusFromSpec(*Spec);

	// 判断能力的状态,如果不是Unlock、Equipped则无法装备
	if (!(
		AbilityStatus.MatchesTagExact(FRPGAuraGameplayTags::Get().Abilities_Status_Unlocked) ||
		AbilityStatus.MatchesTagExact(FRPGAuraGameplayTags::Get().Abilities_Status_Equipped)
	)) { return; }

	MyAsc->ServerEquipAbility(SelectedAbilityTag,SelectedInputSlotTag);

	// 广播假装(值为全none和nullptr)法术菜单中的技能树选择按钮按下,用于取消选中(当按下装备按钮切换完技能之后,选中的按钮会被取消选中)
	GetWidgetControllerParams().GameInstanceSubsystem->OnSpellButtonSelectedChange.Broadcast(nullptr,FGameplayTag(),FGameplayTag(),FGameplayTag());

}
