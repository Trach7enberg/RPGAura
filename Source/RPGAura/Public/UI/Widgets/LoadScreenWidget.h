// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "LoadScreenWidget.generated.h"

class ULoadSlotSwitcherWidget;
class UMVVMLoadSlotSwitcher;
class UMVVMLoadSlot;
/**
 *  加载屏幕Widget
 */
UCLASS()
class RPGAURA_API ULoadScreenWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	/// 初始化LoadSlotSwitcher小部件
	virtual void InitializeLoadSlotSwitcherWidget();

	/// 通过widget标签获取SlotSwitcher的ViewModel
	/// @param WidgetTag 
	/// @return 
	UFUNCTION(BlueprintCallable)
	UMVVMLoadSlotSwitcher* GetSwitcherViewModelByTag(FGameplayTag WidgetTag);

protected:
	/// 用于生成LoadSlotSwitcher的ViewModel类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MVVMViewModel")
	TSubclassOf<UMVVMLoadSlotSwitcher> UmvvmLoadSlotSwitcherClass;

	// 用于生成LoadSlotSwitcher的LoadSlot的ViewModel类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MVVMViewModel")
	TSubclassOf<UMVVMLoadSlot> UmvvmLoadSlotClass;

	/// 加载插槽(切换器)widget
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="LoadSlotSwitcher"))
	TObjectPtr<ULoadSlotSwitcherWidget> LoadSlotSwitcher1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="LoadSlotSwitcher"))
	TObjectPtr<ULoadSlotSwitcherWidget> LoadSlotSwitcher2;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget="LoadSlotSwitcher"))
	TObjectPtr<ULoadSlotSwitcherWidget> LoadSlotSwitcher3;

private:

	/// 加载槽位所对应的的MVVM-ViewModel
	UPROPERTY()
	TMap<FGameplayTag, UMVVMLoadSlotSwitcher*> LoadSlotSwitcherViewModelMap;
};
