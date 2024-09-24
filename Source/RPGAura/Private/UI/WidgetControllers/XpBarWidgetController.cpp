// Copyright GGBAO 


#include "UI/WidgetControllers/XpBarWidgetController.h"

#include "Interfaces/CombatInterface.h"
#include "PlayerStates/BasePlayerState.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

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
	if (!IsWidgetControllerParamsValid()) { return; }

	const auto Ps = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);

	if (!Ps ) { return; }
	
	const auto LocalPlayer = GetWidgetControllerParams().CurrentPlayerController->GetPawn();
	if (!LocalPlayer)
	{
		UE_LOG(UXpBarWidgetControllerLog, Error, TEXT("获取控制器的Pawn失败!"));
		return;
	}

	

	const auto CombatInt = Cast<ICombatInterface>(LocalPlayer);

	if (!CombatInt) { return; }


	const int32 CurrentXp = Xp;

	// 0. 获取当前经验值所对应的等级
	// 1. 获取当前等级满经验值所需要的经验值数
	// 2. 获取升级到当前等级所需的最小经验值数
	// 3. 计算当前经验值所占进度条百分比
	const auto CurrentLevel = GetWidgetControllerParams().GameInstanceSubsystem->GetLevelCorrespondingToXP(
		CombatInt->GetCharacterClass(), CurrentXp,
		CombatInt->GetCharacterLevel());
	float CurrentLevelMaxXP = Ps->GetMaximumXPofLevel(CurrentLevel);

	CurrentLevelMaxXP = (CurrentLevelMaxXP == 0) ? CurrentXp : CurrentLevelMaxXP;

	const float CurrentLevelMinXP = Ps->GetMinimumXpRequiredForLevel(CurrentLevel);

	const UE::Math::TVector2<float> InputMap(CurrentLevelMinXP, CurrentLevelMaxXP);
	const UE::Math::TVector2<float> OutPutMap(0, 1);

	const auto Result = FMath::GetMappedRangeValueClamped(InputMap, OutPutMap, CurrentXp * 1.f);
	UE_LOG(UXpBarWidgetControllerLog, Warning, TEXT("IncomingXP:%d , Min:%.1f , Max:%.1f , Mapped:%f"), Xp,
	       CurrentLevelMinXP,
	       CurrentLevelMaxXP, Result);


	OnGetXp.Broadcast(Result);
}
