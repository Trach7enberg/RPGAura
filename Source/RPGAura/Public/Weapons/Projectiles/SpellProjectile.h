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
	virtual void Destroyed() override;
	

private:
	// 当前投射物有没有Overlap
	bool BIsHit;

	/// 生成特效和声音
	void SpawnVfxAndSound() const;

	TObjectPtr<UAudioComponent> LoopSoundAudioComponent;

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult) override;
};
