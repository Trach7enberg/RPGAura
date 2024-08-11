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


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	TObjectPtr<UNiagaraComponent> FireBoltNiagaraComponent;

	// 投射物冲击时的效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Niagara")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	// 冲击声
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> ImpactSound;

	// 循环声
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> LoopingSound;


	virtual void BeginPlay() override;

private:
	// 当前投射物有没有Overlap
	bool BIsHit;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// 生成特效和声音
	void SpawnVfxAndSound() const;

	TObjectPtr<UAudioComponent> LoopSoundAudioComponent;
};
