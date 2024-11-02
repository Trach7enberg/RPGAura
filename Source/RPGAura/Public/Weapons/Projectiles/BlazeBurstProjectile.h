// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/SpellProjectile.h"
#include "BlazeBurstProjectile.generated.h"

/**
 * 焰爆扩散技能所使用的飞弹
 */
UCLASS()
class RPGAURA_API ABlazeBurstProjectile : public ASpellProjectile
{
	GENERATED_BODY()

public:
	/// 延迟启动飞弹追踪 (飞弹在BeginPlay后才会开始追踪)
	/// @param Delay 
	/// @param HomingActor
	/// @param HomingAcceleration
	virtual void EnableHomingActorWithDelay(const float Delay = .5f, const float HomingAcceleration = 0,const AActor* HomingActor = nullptr);
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

private:

	UPROPERTY()
	FTimerHandle BlazeBurstTimerHandle{};

	UFUNCTION()
	void BlazeBurstTimerCallback();

};
