// Copyright GGBAO 


#include "UI/WidgetControllers/LevelGlobeWidgetController.h"

#include "PlayerStates/BasePlayerState.h"

void ULevelGlobeWidgetController::BindCallBack()
{
	ABasePlayerState* Ps = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);
	if (!Ps) { return; }

	Ps->PlayerLevelChangeDelegate.AddLambda([this](int32 NewLevel, bool IsReallyChange)
	{
		OnLevelChange.Broadcast(NewLevel, IsReallyChange);
	});
}

void ULevelGlobeWidgetController::BroadcastInitialValues()
{
	ABasePlayerState* Ps = Cast<ABasePlayerState>(GetWidgetControllerParams().CurrentPlayerState);
	if (!Ps) { return; }
	Ps->SetPlayerLevel(Ps->GetPlayerLevel(),false);
}
