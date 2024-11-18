// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"


class UAbilitySystemComponent;
class UGameplayEffect;
class USphereComponent;
/**
 * 基础拾取物类
 */
UCLASS()
class RPGAURA_API ABasePickup : public AActor
{
	GENERATED_BODY()

public:
	ABasePickup();

	// 碰撞盒
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> SphereComponent;

	// 静态网格
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UFUNCTION(BlueprintCallable, Category="Pickup|GE")
	void ClientApplyGEToTarget(AActor* Actor, TSubclassOf<UGameplayEffect> GeClass);

	/// 启用拾取物的物理
	void EnablePickUpPhysics(const FVector& Impulsive = FVector{}) const;

	float GetActorLevel() const { return ActorLevel; }

	/// 设置拾取物的等级
	/// @param NewActorLevel 
	void SetActorLevel(const float NewActorLevel) { this->ActorLevel = NewActorLevel; }

protected:
	virtual void BeginPlay() override;

	/// 当前Actor的等级,配合曲线表使用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	float ActorLevel;

	// 拾取物效果是否应用到敌人
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
	bool bEnableEffectsToEnemies;

	// 拾取后是否销毁
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
	bool PickupThenDestroy;

	// 生成拾取物的声音
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|Sound")
	TObjectPtr<USoundBase> PickUpSpawnSound;

	// 拾取物掉落声音
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|Sound")
	TObjectPtr<USoundBase> PickUpHitSound;

	// 拾取物消耗声音
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|Sound")
	TObjectPtr<USoundBase> PickUpConsumeSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|GE")
	TSubclassOf<UGameplayEffect> CurrentGameplayEffectClass;

	UFUNCTION()
	void OnBeginOverBegin(UPrimitiveComponent* OverlappedComponent,
	                      AActor*              OtherActor,
	                      UPrimitiveComponent* OtherComp,
	                      int32                OtherBodyIndex,
	                      bool                 bFromSweep,
	                      const FHitResult&    SweepResult);

	UFUNCTION()
	void OnEndOverBegin(UPrimitiveComponent* OverlappedComponent,
	                    AActor*              OtherActor,
	                    UPrimitiveComponent* OtherComp,
	                    int32                OtherBodyIndex);

private:
	FActiveGameplayEffectHandle CurrentActiveGameplayEffectHandle;

	/// 拾取物落地没有
	UPROPERTY()
	bool bIsOnGround = false;
};
