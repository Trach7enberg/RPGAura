// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interfaces/HighLightInterface.h"
#include "Interfaces/SaveInterface.h"
#include "CheckPointPlayerStart.generated.h"

class UBoxComponent;
/**
 *  检查点PlayerStart
 */
UCLASS()
class RPGAURA_API ACheckPointPlayerStart : public APlayerStart, public ISaveInterface,public IHighLightInterface
{
	GENERATED_BODY()

public:
	ACheckPointPlayerStart(const FObjectInitializer& ObjectInitializer);

	/// 角色到达检查点时候的事件
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CheckPointReached();

	/// 当前检查点是否已经到达 (带有SaveGame标识很重要!)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, SaveGame)
	bool Reached = false;

	// ~SaveInterface
	virtual bool ShouldLoadTransform() override;
	virtual void LoadActorSaveInfo() override;
	
	// ~HighLightInterface
	UFUNCTION(BlueprintCallable)
	virtual void HighLightActor() override;
	UFUNCTION(BlueprintCallable)
	virtual void UnHighLightActor() override;

	virtual void GetMoveToLocation(FVector& OutDestination) override;
	// ~HighLightInterface

protected:
	/// Mesh的描边颜色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Level")
	float CustomDepthStencilValue = 252.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Level")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Level")
	TObjectPtr<UBoxComponent> BoxCollision;

	/// 标记自动行走位置的箭头组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Level")
	TObjectPtr<USceneComponent> AutoMovePosition;

	/// 检查点的发光材质
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Level")
	TObjectPtr<UMaterialInterface> CheckPointMi;

	/// 给Mesh设置的动态材质,用于更改发光
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Level")
	TObjectPtr<UMaterialInstanceDynamic> LocalCheckPointDynamicMi;

	virtual void BeginPlay() override;

	virtual void HandelGlowMaterial();

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor*              OtherActor,
		UPrimitiveComponent* OtherComp,
		int32                OtherBodyIndex,
		bool                 bFromSweep,
		const FHitResult&    SweepResult);

	UFUNCTION()
	virtual void OnCursorBegin(UPrimitiveComponent* TouchedComponent);

	UFUNCTION()
	virtual void OnCursorEnd(UPrimitiveComponent* TouchedComponent);
};
