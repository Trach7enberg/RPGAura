// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "BaseWidgetController.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;


/**
 *  基础UI控制器,用来获得数据以传递给显示的UI
 */
UCLASS(BlueprintType, Blueprintable)
class RPGAURA_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="ControllerParas")
	void SetWidgetControllerParams(const FWidgetControllerParams &Params)
	{
		WidgetControllerParams = Params;
	}

	UFUNCTION(BlueprintCallable, Category="ControllerParas")
	FWidgetControllerParams GetWidgetControllerParams() const
	{
		return WidgetControllerParams;
	}

	/// 广播初始值
	UFUNCTION(BlueprintCallable, Category="Broadcast")
	virtual void BroadcastInitialValues();

	/// 控制器的基本参数是否有效
	/// @return 有效则为true
	bool IsWidgetControllerParamsValid() const;

	/// 绑定一系列的回调函数,用于当前控制器的Owner的GAS中的属性值更改时绑定的回调函数
	/// 一般在创建控制器(UWidgetControllerBpFuncLib::CreateWidgetController)中自动调用
	virtual void BindCallBack();

private:
	FWidgetControllerParams WidgetControllerParams;


};
