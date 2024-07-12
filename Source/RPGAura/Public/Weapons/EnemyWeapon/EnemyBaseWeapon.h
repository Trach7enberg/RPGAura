// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/HighLightInterface.h"
#include "Weapons/BaseWeapon.h"
#include "EnemyBaseWeapon.generated.h"

/**
 * 敌人武器的基类
 */
UCLASS()
class RPGAURA_API AEnemyBaseWeapon : public ABaseWeapon, public IHighLightInterface
{
	GENERATED_BODY()

public:
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
};
