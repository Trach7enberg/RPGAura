// Copyright GGBAO 


#include "Actor/AuraEnemySpawnPoint.h"

#include "Characters/EnemyCharacter.h"
#include "CoreTypes/RPGAuraCoreTypes.h"

DEFINE_LOG_CATEGORY_STATIC(AAuraEnemySpawnPointLog, All, All);

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	if (!GetWorld()) { return; }
	
	if (!EnemyClass)
	{
		UE_LOG(AAuraEnemySpawnPointLog, Error, TEXT("敌人生产类无效!!"));
		return;
	}

	FActorSpawnParameters SpawnParameters{};
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const auto Enemy = GetWorld()->SpawnActorDeferred<AEnemyCharacter>(EnemyClass, GetActorTransform());
	if (!Enemy) { return; }
	Enemy->SetEnemyLevel(EnemyLevel);
	Enemy->SetEnemyCharacterClass(EnemyCass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}

AAuraEnemySpawnPoint::AAuraEnemySpawnPoint() { EnemyCass = ECharacterClass::Warrior; }
