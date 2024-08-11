// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "WidgetControllerBpFuncLib.generated.h"

class UAttributeMenuWidgetController;
class UMainWidgetController;
class UBaseWidgetController;
struct FWidgetControllerParams;
/**
 * Widget控制器的蓝图函数库
 */
UCLASS()
class RPGAURA_API UWidgetControllerBpFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
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

	/// 根据玩家控制器获取主Widget的小部件控制器
	/// @param PlayerController 玩家控制器
	/// @return MainWidgetController
	UFUNCTION(BlueprintPure, Category="WidgetController")
	static UMainWidgetController* GetMainWidgetController(const APlayerController* PlayerController);

	/// 设置Hud里的属性菜单小部件控制器的引用
	/// @param PlayerController 玩家控制器
	/// @param AttributeMenuWidgetController 属性菜单小部件控制器
	UFUNCTION(BlueprintCallable, Category="WidgetController")
	static void SetHudAttributeMenuWidgetController(const APlayerController* PlayerController,
	                                                UAttributeMenuWidgetController* AttributeMenuWidgetController);

	/// 根据玩家控制器获取主Widget里的属性菜单小部件控制器
	/// @param PlayerController 玩家控制器
	/// @return AttributeMenuWidgetController
	UFUNCTION(BlueprintPure, Category="WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const APlayerController* PlayerController);
};
