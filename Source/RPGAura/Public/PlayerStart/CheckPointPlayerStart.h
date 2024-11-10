// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CheckPointPlayerStart.generated.h"

class UBoxComponent;
/**
 *  检查点PlayerStart
 */
UCLASS()
class RPGAURA_API ACheckPointPlayerStart : public APlayerStart
{
	GENERATED_BODY()

public:
	ACheckPointPlayerStart(const FObjectInitializer& ObjectInitializer);

	/// 角色到达检查点时候的事件
	UFUNCTION(BlueprintCallable,BlueprintImplementableEvent)
	void CheckPointReached();
	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Level")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category="Level")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,  Category="Level")
	TObjectPtr<UMaterialInterface> CheckPointMi;

	virtual void BeginPlay() override;

	void HandleMeshMaterial();
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                             const FHitResult& SweepResult);
};
