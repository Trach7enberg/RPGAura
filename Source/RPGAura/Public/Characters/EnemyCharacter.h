// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/HighLightInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 所有敌人的基类
 */
UCLASS()
class RPGAURA_API AEnemyCharacter : public ACharacterBase, public IHighLightInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

protected:
	virtual void BeginPlay() override;

private:
	// ~ IHighLightInterface Begin
	UFUNCTION()
	void OnMouseOver(AActor *TouchedActor);


	UFUNCTION()
	void EndMouseOver(AActor *TouchedActor);
	// ~IHighLightInterface End
};
