// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/BaseProjectile.h"
#include "SpellProjectile.generated.h"

/**
 * 魔法飞弹类
 */
UCLASS()
class RPGAURA_API ASpellProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	ASpellProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	TObjectPtr<UNiagaraComponent> FireBoltNiagaraComponent;

	

	
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	

private:
	// 当前投射物有没有Overlap
	bool BIsHit;

	virtual void SpawnVfxAndSound() const override;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;
};
