// Copyright GGBAO 


#include "GameModes/RPGAuraGameModeBase.h"

#include "Characters/AuraCharacter.h"
#include "Controller/BasePlayerController.h"

ARPGAuraGameModeBase::ARPGAuraGameModeBase()
{
	DefaultPawnClass = AAuraCharacter::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
}
