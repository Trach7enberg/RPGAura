// Copyright GGBAO 


#include "Characters/EnemyGoblinCharacter.h"

#include "Components/CapsuleComponent.h"


DEFINE_LOG_CATEGORY_STATIC(MyAEnemyGoblinCharacterLog,All,All);
AEnemyGoblinCharacter::AEnemyGoblinCharacter()
{
	if (GetMesh())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(51.f);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -50));
	}
	
}

