// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMCMaxHealth.generated.h"

/**
 * 用于计算生命最大值的magnitude的MMC类
 */
UCLASS()
class RPGAURA_API UMMCMaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMCMaxHealth();

	/// MMC计算的结果在这个函数返回
	/// @param Spec 
	/// @return 最终的幅度值
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const override;

private:
	float BaseValue = 100.f;
	// 要捕获的属性,这个类型是一个结构体
	FGameplayEffectAttributeCaptureDefinition CaptureAttributeDef{};
};
