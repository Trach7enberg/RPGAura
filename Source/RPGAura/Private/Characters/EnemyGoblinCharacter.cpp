// Copyright GGBAO 


#include "Characters/EnemyGoblinCharacter.h"

#include "Components/CapsuleComponent.h"

AEnemyGoblinCharacter::AEnemyGoblinCharacter()
{
	if (GetMesh())
	{
		GetCapsuleComponent()->SetCapsuleHalfHeight(51.f);
		GetMesh()->SetRelativeLocation(FVector(0, 0, -50));
	}

}
