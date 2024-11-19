// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "PlayerStart/CheckPointPlayerStart.h"
#include "CheckPointBeacon.generated.h"

/**
 * 信标PlayerStart,不用作重生点
 */
UCLASS()
class RPGAURA_API ACheckPointBeacon : public ACheckPointPlayerStart
{
	GENERATED_BODY()

public:
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

protected:
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor*              OtherActor,
		UPrimitiveComponent* OtherComp,
		int32                OtherBodyIndex,
		bool                 bFromSweep,
		const FHitResult&    SweepResult) override;
};
