// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "BaseSummonAbility.generated.h"

/**
 * 基础召唤能力类
 */
UCLASS()
class RPGAURA_API UBaseSummonAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UBaseSummonAbility();

	

	// 召唤物距离目标的距离
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Summon")
	float AcceptableMaxRadius;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Summon")
	float AcceptableMinRadius;

	// 召唤物分散在目标旁边的角度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Summon")
	float SpawnSpread;

	// 要召唤的召唤物实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Summon")
	TArray<TSubclassOf<APawn>> SummonClasses;


	/// 获取召唤物生成的位置
	/// TODO 待修改为用EQS进行获取环绕目标有效位置
	/// @return 
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation();

	/// 生成召唤物
	/// @param Location 召唤物位置
	/// @param bRandomChoseSummonClass 随机从召唤物实体类数组中挑选召唤物
	/// @return 
	UFUNCTION(BlueprintCallable)
	const AActor* SpawnSummon(const FVector  & Location, bool bRandomChoseSummonClass = true);

private:

	// 召唤的人数
	int32 SpawnNum;

	// 当前生成的召唤物索引
	int32 CurrentSpawnSummonIndex ;
	
	// 绘制夹角线和调试球
	void DrawDebug(const FVector& Location, const FVector& TmpDeltaSpread, const FVector& RandomLocOnLine,
	               float RangeSphereRadius, float SpawnSphereRadius, float RangeDuration = 1.5,
	               float SpawnDuration = 1.5) const;

	UFUNCTION()
	void SpawnActorOnDestroy( AActor*DestroyedActor);
};
