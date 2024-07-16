// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DevGEActor.generated.h"

class UGameplayEffect;

UCLASS()
class RPGAURA_API ADevGEActor : public AActor
{
	GENERATED_BODY()

public:
	ADevGEActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass = nullptr;

	/// 应用GameplayEffects到目标
	/// @param Actor 目标对象
	/// @param Ge 游戏效果类
	UFUNCTION(BlueprintCallable, Category="Dev GE")
	void ApplyGEToTarget(AActor *Actor, TSubclassOf<UGameplayEffect> Ge) const;

public:
	virtual void Tick(float DeltaTime) override;

};
