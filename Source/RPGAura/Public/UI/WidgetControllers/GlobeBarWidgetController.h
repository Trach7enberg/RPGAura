// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "GlobeBarWidgetController.generated.h"

struct FOnAttributeChangeData;
/**
 * 圆球(比如血球和蓝球)Widget的控制器
 */
UCLASS()
class RPGAURA_API UGlobeBarWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	/// 广播GAS中的生命值变化,bool为true时候表明生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnVitalAttributeChangedSignature OnHealthChangedSignature;

	/// 广播GAS中的最大生命值变化,bool为true时候表明最大生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnVitalAttributeChangedSignature OnMaxHealthChangedSignature;

	/// 广播GAS中的魔力值变化,bool为true时候表明魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnVitalAttributeChangedSignature OnManaChangedSignature;

	/// 广播GAS中的最大魔力值变化,bool为true时候表明最大魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnVitalAttributeChangedSignature OnMaxManaChangedSignature;

	/// 广播 数据表中的表行,表行的类型为FUIWidgetRow
	UPROPERTY(BlueprintAssignable, Category="GAS | Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRow;

	

	/// 绑定回调到GAS系统
	virtual void BindCallBack() override;

	/// 广播初始值,以便UI做出相应的响应
	virtual void BroadcastInitialValues() override;

private:
	void HealthChanged(const FOnAttributeChangeData &Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData &Data) const;
	void ManaChanged(const FOnAttributeChangeData &Data) const;
	void MaxManaChanged(const FOnAttributeChangeData &Data) const;
};
