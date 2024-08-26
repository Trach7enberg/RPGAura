// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
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

	/// 用于造成伤害的GeSpecHandle
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn=true), Category="GameplayEffect")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

	virtual void Destroyed() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileLifeSpawn = 5.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	virtual void BeginPlay() override;

private:
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp,
	                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	
};
