// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "DevGEActor.generated.h"

class UAbilitySystemComponent; 
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
	void ApplyGEToTarget(AActor *Actor, TSubclassOf<UGameplayEffect> Ge) ;

	UFUNCTION(BlueprintCallable, Category="Dev GE")
	void OnOverLap(AActor *TargetActor) ;

	UFUNCTION(BlueprintCallable, Category="Dev GE")
	void EndOverLap(AActor *TargetActor, bool DestroyActor = false) ;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	TSubclassOf<UGameplayEffect> CurrentGameplayEffectClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	EGeApplicationPolicy CurrentApplicationPolicy = EGeApplicationPolicy::ApplyOnOverlap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dev GE")
	EGeRemovalPolicy InfinityRemovalPolicy = EGeRemovalPolicy::RemoveOnEndOverlap;

	
	/// 存储当前正在启用的GameplayEffect,一个角色的ASC可以存在多个不同已经启用的效果
	TMap<UAbilitySystemComponent *,TSet<FActiveGameplayEffectHandle>> ActiveGeMap;
};
