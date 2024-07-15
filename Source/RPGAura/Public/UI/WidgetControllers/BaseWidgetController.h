// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetController.generated.h"


class UAttributeSet;
class UAbilitySystemComponent;

/**
 *  Widget控制器需要的基本参数的结构体
 */
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController *Pc,
	                        APlayerState *Ps,
	                        UAbilitySystemComponent *Asc,
	                        UAttributeSet *As) :
		CurrentPlayerController(Pc), CurrentPlayerState(Ps),
		CurrentAbilitySystemComponent(Asc), CurrentAttributeSet(As) {}

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> CurrentPlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> CurrentPlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> CurrentAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> CurrentAttributeSet = nullptr;
};

/**
 *  基础UI控制器,用来获得数据以传递给显示的UI
 */
UCLASS(BlueprintType, Blueprintable)
class RPGAURA_API UBaseWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="ControllerParas")
	void SetWidgetControllerParams(const FWidgetControllerParams &Params) { WidgetControllerParams = Params; }

	UFUNCTION(BlueprintCallable, Category="ControllerParas")
	FWidgetControllerParams GetWidgetControllerParams() const { return WidgetControllerParams; }

	/// 广播初始值
	UFUNCTION(BlueprintCallable, Category="Broadcast")
	virtual void BroadcastInitialValues();

	/// 控制器的基本参数是否有效
	/// @return 有效则为true
	bool IsWidgetControllerParamsValid() const;

private:
	FWidgetControllerParams WidgetControllerParams;


};
