// Copyright GGBAO 


#include "UI/WidgetControllers/XpBarWidgetController.h"

#include "PlayerStates/BasePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(UXpBarWidgetControllerLog, All, All);

void UXpBarWidgetController::BindCallBack()
{
	ABasePlayerState* Ps = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);
	if (!Ps) { return; }

	Ps->PlayerXpChangeDelegate.AddUObject(this, &UXpBarWidgetController::CalculateXpPercent);
}

void UXpBarWidgetController::BroadcastInitialValues()
{
	const auto Ps = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);
	if (!Ps) { return; }

	// 广播初始经验值
	Ps->SetPlayerXP(Ps->GetPlayerCurrentXP());
}

void UXpBarWidgetController::CalculateXpPercent(int32 Xp)
{
	if (!IsWidgetControllerParamsValid()) { return ; }

	const auto Ps = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);
	if (!Ps) { return; }

	const int32 CurrentXp = Xp;

	// 1. 获取当等级满经验值所需要的经验值数
	// 2. 获取升级到当前等级所需的最小经验值数
	// 3. 计算当前经验值所占进度条百分比
	const float CurrentLevelMaxXP = Ps->GetMaximumXPofLevel(Ps->GetPlayerLevel());
	const float CurrentLevelMinXP = Ps->GetMinimumXpRequiredForLevel(Ps->GetPlayerLevel());

	const UE::Math::TVector2<float> InputMap(CurrentLevelMinXP, CurrentLevelMaxXP);
	const UE::Math::TVector2<float> OutPutMap(0, 1);

	const auto Result = FMath::GetMappedRangeValueClamped(InputMap, OutPutMap, CurrentXp * 1.f);
	UE_LOG(UXpBarWidgetControllerLog, Warning, TEXT("Min:%.1f , Max:%.1f , Mapped:%.1f"), CurrentLevelMinXP,
	       CurrentLevelMaxXP, Result);


	OnGetXp.Broadcast(Result);
}
