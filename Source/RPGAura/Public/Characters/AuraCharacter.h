// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
/**
 * Aura人物类
 */
UCLASS()
class RPGAURA_API AAuraCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Camera")
	UCameraComponent * CameraComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Camera")
	USpringArmComponent* SpringArmComponent;
};
