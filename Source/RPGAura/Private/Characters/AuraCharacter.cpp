// Copyright GGBAO 


#include "Characters/AuraCharacter.h"


AAuraCharacter::AAuraCharacter()
{
	if (GetMesh()) { GetMesh()->SetRelativeLocation(FVector(0, 0, -85)); }
}
