// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetControllerBpFuncLib.generated.h"

struct FWidgetControllerParams;
class UBaseWidgetController;
/**
 * Widget控制器的蓝图函数库
 */
UCLASS()
class RPGAURA_API UWidgetControllerBpFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UWidgetControllerBpFuncLib();

	/// 获取创建Widget控制器所需要的基本参数
	/// @param OwnerController Widget控制器的拥有者(得是玩家控制器!)
	/// @param Params 控制器的基本参数
	UFUNCTION(BlueprintPure, Category="WidgetController")
	static void CreateWidgetControllerParams(AController* OwnerController, FWidgetControllerParams& Params);

	/// 创建Widget的控制器 
	/// @param WidgetControllerClass 控制器的Class
	/// @param OwnerController 控制器的Owner(得是玩家控制器!)
	/// @return T类型的Widget控制器
	UFUNCTION(BlueprintPure, Category="WidgetController")
	static UBaseWidgetController* CreateWidgetController(TSubclassOf<UBaseWidgetController> WidgetControllerClass,
	                                                     AController* OwnerController);


	
	
};
