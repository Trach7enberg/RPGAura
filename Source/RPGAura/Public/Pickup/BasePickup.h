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
	

protected:
	virtual void BeginPlay() override;

	/// 当前Actor的等级,配合曲线表使用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
	float ActorLevel ;

	// 拾取物效果是否应用到敌人
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
	bool bEnableEffectsToEnemies ;

	// 拾取后是否销毁
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
	bool PickupThenDestroy ;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup|GE")
	TSubclassOf<UGameplayEffect> CurrentGameplayEffectClass;
	
	UFUNCTION()
	void OnBeginOverBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp,
						  int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OnEndOverBegin(UPrimitiveComponent*OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	FActiveGameplayEffectHandle CurrentActiveGameplayEffectHandle;
};
