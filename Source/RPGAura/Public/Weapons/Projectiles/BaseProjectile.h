// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
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
	FDamageEffectParams DamageEffectParams;
	
	UProjectileMovementComponent* GetProjectileMovementComponent() const
	{
		return ProjectileMovementComponent.Get();
	}
	
	/// 启用当飞弹的追踪目标被销毁时候的事件
	virtual void EnableHomingActorOnDestroyedEvent();

	virtual void Destroyed() override;

	virtual void LifeSpanExpired() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float ProjectileLifeSpawn = 5.0f;

	// 投射物冲击时的效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Niagara")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	// 冲击声
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Sound")
	TObjectPtr<USoundBase> ImpactSound;

	// 循环声
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile|Sound")
	TObjectPtr<USoundBase> LoopingSound;
	
	// 飞弹是否忽略友军
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	bool bIgnoreFriend;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	/// 投射物身上的循环声音
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopSoundAudioComponent;
	
	virtual void BeginPlay() override;

	/// 生成特效和声音
	virtual void SpawnVfxAndSound() const;

private:
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp,
	                             int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/// 当飞弹的追踪目标被销毁时回调的事件
	/// @param Actor 
	UFUNCTION()
	virtual void HomingActorOnPreDestroyed(AActor* Actor);
};
