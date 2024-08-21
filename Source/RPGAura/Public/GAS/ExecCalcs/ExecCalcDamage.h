// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalcDamage.generated.h"





/**
 * 自定义执行计算伤害类
 */
UCLASS()
class RPGAURA_API UExecCalcDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalcDamage();

	// 抗性属性标签对应的捕获Def

	
	
	/// 执行计算类
	/// @param ExecutionParams 
	/// @param OutExecutionOutput 
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	                                    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
