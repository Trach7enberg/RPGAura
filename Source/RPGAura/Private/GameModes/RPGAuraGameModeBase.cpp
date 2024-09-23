// Copyright GGBAO 


#include "GameModes/RPGAuraGameModeBase.h"

#include "Characters/AuraCharacter.h"
#include "Controller/BasePlayerController.h"

DEFINE_LOG_CATEGORY_STATIC(ARPGAuraGameModeBaseLog,All,All);

ARPGAuraGameModeBase::ARPGAuraGameModeBase()
{
	DefaultPawnClass = AAuraCharacter::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
}
