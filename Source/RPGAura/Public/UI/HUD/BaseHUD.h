// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UMainWidgetController;
class UBaseUserWidget;
struct FWidgetControllerParams;


/**
 * HUD类
 */
UCLASS()
class RPGAURA_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	/// 获取当前主要widget的控制器
	/// @return 当前主要widget的控制器
	UFUNCTION(BlueprintCallable, Category="HUD")
	UMainWidgetController *GetMainWidgetController() const;

	/// 初始化HUD的主要widget、并设置主要widget的控制器
	void InitHudMainWidget();

protected:
	virtual void BeginPlay() override;

	/// 用于生成主要widget的Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WidgetClass")
	TSubclassOf<UBaseUserWidget> MainWidgetClass;

	/// 用于生成主要widget控制器的Class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WidgetClass")
	TSubclassOf<UMainWidgetController> MainWidgetControllerClass = nullptr;

private:
	/// 当前主Widget的引用
	TObjectPtr<UBaseUserWidget> CurrentMainWidget = nullptr;

	// 当前主要widget的控制器
	TObjectPtr<UMainWidgetController> CurrentMainWidgetController = nullptr;

	/// 获取创建控制器所需要的基本参数
	/// @param Params 控制器的基本参数
	void CreateWidgetControllerParams(FWidgetControllerParams &Params) const;

	/// 创建主要Widget的控制器
	/// @return 主要Widget的控制器
	UMainWidgetController *CreateMainWidgetController();
};
