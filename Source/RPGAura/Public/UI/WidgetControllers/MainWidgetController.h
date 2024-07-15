// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "MainWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, bool, /* 增加还是减少,增加则为true */
                                             BIsIncreased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthChangedSignature, float, NewMaxHealth,
                                             bool, /* 增加还是减少,增加则为true */ BIsIncreased);

/**
 * 主要widget的控制器类
 */
UCLASS()
class RPGAURA_API UMainWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	/// 广播初始值
	virtual void BroadcastInitialValues() override;

	/// 广播GAS中的生命值变化,bool为true时候表明生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnHealthChangedSignature OnHealthChangedSignature;

	/// 广播GAS中的最大生命值变化,bool为true时候表明最大生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChangedSignature;


};
