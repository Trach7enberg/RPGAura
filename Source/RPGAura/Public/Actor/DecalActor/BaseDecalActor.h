// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDecalActor.generated.h"

/**
 * 基础贴花类
 */
UCLASS()
class RPGAURA_API ABaseDecalActor : public AActor
{
	GENERATED_BODY()

public:
	ABaseDecalActor();

	/// 设置贴花的可见性
	/// @param bVisible 
	void SetDecalActorVisibility(const bool bVisible) const { GetRootComponent()->SetVisibility(bVisible, true); }

	/// 
	/// @param DecalMaterial 
	void SetDecalMaterial(UMaterialInterface* DecalMaterial) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UDecalComponent> DecalComponent;
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
