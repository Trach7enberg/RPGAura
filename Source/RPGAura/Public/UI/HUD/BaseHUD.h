// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

class UAttributeMenuWidgetController;
class UBaseWidgetController;
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
    FORCEINLINE UMainWidgetController* GetMainWidgetController() const;

    UFUNCTION(BlueprintCallable, Category="HUD")
    FORCEINLINE UAttributeMenuWidgetController* GetAttributeMenuWidgetController() const;

    /// 初始化HUD的主要widget、并设置主要widget的控制器、初始化所有UI的默认值
    void InitHudMainWidget();

protected:
    virtual void BeginPlay() override;

    /// 用于生成主要widget的Class
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WidgetClass")
    TSubclassOf<UBaseUserWidget> MainWidgetClass;

    /// 用于生成主要widget控制器的Class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WidgetClass")
    TSubclassOf<UMainWidgetController> MainWidgetControllerClass;

    /// 用于生成属性菜单widget控制器的Class
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="WidgetClass")
    TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

private:
    /// 当前主Widget的引用
    TObjectPtr<UBaseUserWidget> CurrentMainWidget;

    TObjectPtr<UMainWidgetController> CurrentMainWidgetController;

    TObjectPtr<UAttributeMenuWidgetController> CurrentAttributeMenuWidgetController;
};