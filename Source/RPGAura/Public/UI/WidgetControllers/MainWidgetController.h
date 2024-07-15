// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "MainWidgetController.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, NewHealth, bool,
                                             /* Health增加还是减少,增加则为true */
                                             BIsIncreased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxHealthChangedSignature, float, NewMaxHealth,
                                             bool, /* MaxHealth增加还是减少,增加则为true */ BIsIncreased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnManaChangedSignature, float, NewMana, bool, /* Mana增加还是减少,增加则为true */
                                             BIsIncreased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMaxManaChangedSignature, float, NewMaxMana,
                                             bool, /* MaxMana增加还是减少,增加则为true */ BIsIncreased);

/**
 * 主要widget的控制器类
 */
UCLASS()
class RPGAURA_API UMainWidgetController  : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	/// 广播GAS中的生命值变化,bool为true时候表明生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnHealthChangedSignature OnHealthChangedSignature;

	/// 广播GAS中的最大生命值变化,bool为true时候表明最大生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChangedSignature;

	/// 广播GAS中的魔力值变化,bool为true时候表明魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnManaChangedSignature OnManaChangedSignature;

	/// 广播GAS中的最大魔力值变化,bool为true时候表明最大魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnMaxManaChangedSignature OnMaxManaChangedSignature;

	/// 广播初始值,以便UI做出相应的响应
	virtual void BroadcastInitialValues() override;

	/// 绑定回调到GAS系统
	virtual void BindCallBackToGas() override;

private:
	void HealthChanged(const FOnAttributeChangeData &Data) const;

	void MaxHealthChanged(const FOnAttributeChangeData &Data) const;

	void ManaChanged(const FOnAttributeChangeData &Data) const;

	void MaxManaChanged(const FOnAttributeChangeData &Data) const;
};
