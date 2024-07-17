// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DevGEActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EGeApplicationPolicy
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DontApply,
};

UENUM(BlueprintType)
enum class EGeRemovalPolicy
{
	RemoveOnEndOverlap,
	DontRemove,
};

UCLASS()
class RPGAURA_API ADevGEActor : public AActor
{
	GENERATED_BODY()

public:
	ADevGEActor();

	/// 应用GameplayEffects到目标
	/// @param Actor 目标对象
	/// @param Ge 游戏效果类
	UFUNCTION(BlueprintCallable, Category="Dev GE")
	void ApplyGEToTarget(AActor *Actor, TSubclassOf<UGameplayEffect> Ge) const;

	UFUNCTION(BlueprintCallable, Category="Dev GE")
	void OnOverLap(AActor *TargetActor);

	UFUNCTION(BlueprintCallable, Category="Dev GE")
	void EndOverLap(AActor *TargetActor, bool DestroyActor = false);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	TSubclassOf<UGameplayEffect> InfinityGameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	EGeApplicationPolicy InstantApplicationPolicy = EGeApplicationPolicy::ApplyOnOverlap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	EGeApplicationPolicy DurationApplicationPolicy = EGeApplicationPolicy::ApplyOnOverlap;;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	EGeApplicationPolicy InfinityApplicationPolicy = EGeApplicationPolicy::ApplyOnOverlap;;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	EGeRemovalPolicy InfinityRemovalPolicy = EGeRemovalPolicy::RemoveOnEndOverlap;


};
