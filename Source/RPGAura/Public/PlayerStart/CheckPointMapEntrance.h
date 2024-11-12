// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "PlayerStart/CheckPointPlayerStart.h"
#include "CheckPointMapEntrance.generated.h"

/**
 *  用于"旅行"地图的检查点
 */
UCLASS()
class RPGAURA_API ACheckPointMapEntrance : public ACheckPointPlayerStart
{
	GENERATED_BODY()
public:
	// ~SaveInterface
	virtual void LoadActorSaveInfo() override;	
	// ~SaveInterface
	
protected:
	/// 旅行目的地 TODO 待修改应该从游戏模式获取? 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	TSoftObjectPtr<UWorld> DestinationMap{};

	/// 旅行目的地的默认玩家出生点
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Level")
	FName DestinationPlayerStartTag{};

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
	                             AActor*              OtherActor,
	                             UPrimitiveComponent* OtherComp,
	                             int32                OtherBodyIndex,
	                             bool                 bFromSweep,
	                             const FHitResult&    SweepResult) override;
};
