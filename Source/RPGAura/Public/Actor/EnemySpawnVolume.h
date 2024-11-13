// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SaveInterface.h"
#include "EnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;
/*
 * 用于生成敌人的volume
 */
UCLASS()
class RPGAURA_API AEnemySpawnVolume : public AActor,public ISaveInterface
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnVolume();

	// ~ SaveInterface
	virtual bool ShouldLoadTransform() override;
	virtual void LoadActorSaveInfo() override;
	virtual void GetMoveToLocation(FVector& OutDestination) override;
	// ~ SaveInterface

	/// 当前点位是否已经到达过
	UPROPERTY(BlueprintReadOnly,SaveGame)
	bool bReached = false;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Spawn")
	TObjectPtr<UBoxComponent> BoxVolume;

	/// 生成点
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spawn")
	TArray<AAuraEnemySpawnPoint*> SpawnPoints{};

	UFUNCTION()
	virtual void OnSphereOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor*              OtherActor,
		UPrimitiveComponent* OtherComp,
		int32                OtherBodyIndex,
		bool                 bFromSweep,
		const FHitResult&    SweepResult) ;

};
