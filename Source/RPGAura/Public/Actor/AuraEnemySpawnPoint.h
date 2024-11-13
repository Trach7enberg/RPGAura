// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AuraEnemySpawnPoint.generated.h"

enum class ECharacterClass : uint8;
class AEnemyCharacter;
/**
 * 用于生成敌人的点
 */
UCLASS()
class RPGAURA_API AAuraEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	/// 生成敌人
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

protected:
	AAuraEnemySpawnPoint();
	/// 用于生成敌人的Class
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Enemy")
	TSubclassOf<AEnemyCharacter> EnemyClass;

	/// 敌人等级
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Enemy")
	int32 EnemyLevel = 1;

	/// 敌人的职业
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Enemy")
	ECharacterClass EnemyCass;
};
