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

	/// 获取召唤物生成的位置
	/// TODO 待修改为用EQS进行获取有效位置
	/// @return 
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation();

	// 召唤的人数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Summon")
	int32 SpawnNum;

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
	TArray<TSubclassOf<APawn>> SummonClass;

private:
	// 绘制夹角线和调试球
	void DrawDebug(const FVector& Location, const FVector& TmpDeltaSpread, const FVector& RandomLocOnLine,
	               float RangeSphereRadius, float SpawnSphereRadius, float RangeDuration = 1.5,
	               float SpawnDuration = 1.5) const;
};
