// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "FireArea.generated.h"

class UBoxComponent;
class UGameplayEffect;
/**
 * 火焰区域 (重叠时无限效果)
 */
UCLASS()
class RPGAURA_API AFireArea : public AActor
{
	GENERATED_BODY()

public:
	AFireArea();
	
	UFUNCTION(BlueprintCallable, Category="FireArea|GE")
	void ClientApplyGEToTarget(AActor* Actor, TSubclassOf<UGameplayEffect> GeClass);

protected:
	virtual void BeginPlay() override;

	/// 当前Actor的等级,配合曲线表使用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FireArea")
	float ActorLevel ;

	// 效果是否应用到敌人
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FireArea")
	bool bEnableEffectsToEnemies ;

	// 火焰区域是否销毁
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FireArea")
	bool OverlapThenDestroy ;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FireArea|GE")
	TSubclassOf<UGameplayEffect> CurrentGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="FireArea|GE")
	TObjectPtr<UBoxComponent> BoxComponent;
	
	UFUNCTION()
	void OnBeginOverBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
						  int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnEndOverBegin(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FActiveGameplayEffectHandle CurrentActiveGameplayEffectHandle;

	/// 存储当前正在启用的GameplayEffect,一个角色的ASC可以存在多个不同已经启用的效果
	TMap<UAbilitySystemComponent*, TSet<FActiveGameplayEffectHandle>> ActiveGeMap;
	
};
