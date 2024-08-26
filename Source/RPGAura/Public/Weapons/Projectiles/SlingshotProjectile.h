// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/BaseProjectile.h"
#include "SlingshotProjectile.generated.h"

/**
 * 哥布林的投石
 */
UCLASS()
class RPGAURA_API ASlingshotProjectile : public ABaseProjectile
{
	GENERATED_BODY()


public:
	ASlingshotProjectile();

protected:
	virtual void BeginPlay() override;

	/// 飞弹忽视友军
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Projectile")
	bool bIgnoreFriendly;
private:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;
};
