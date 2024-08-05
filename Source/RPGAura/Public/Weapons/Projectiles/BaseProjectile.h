// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;
/*
 *  基础投弹类
 */
UCLASS()
class RPGAURA_API ABaseProjectile : public AActor
{
    GENERATED_BODY()

public:
    ABaseProjectile();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
    TObjectPtr<USphereComponent> SphereComponent;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;


    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
    TObjectPtr<UNiagaraComponent> FireBoltNiagaraComponent;

    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};