// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "MVVMLoadScreen.generated.h"

class UBaseGameInstance;
class ARPGAuraGameModeBase;
class UMVVMLoadSlotSwitcher;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotSelected, UMVVMLoadSlotSwitcher*, LoadSlotSwitcherViewModel);

/**
 * 用于加载屏幕的MVVM架构的ViewModel,(响应插槽按钮)
 */
UCLASS()
class RPGAURA_API UMVVMLoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/// 选择了合法插槽后的委托
	UPROPERTY(BlueprintAssignable)
	FSlotSelected OnSlotSelected;

	/// 初始化插槽切换器的ViewModel
	void InitialLoadSlotSwitcherWidgetViewModel();

	/// 通过widget标签获取SlotSwitcher的ViewModel
	/// @param LoadSlotSwitchWidgetTag 
	/// @return 
	UFUNCTION(BlueprintCallable)
	UMVVMLoadSlotSwitcher* GetSwitcherViewModelByTag(FGameplayTag LoadSlotSwitchWidgetTag);

	/// 新存档按钮按下时
	/// @param LoadSlotSwitcherViewModel 
	/// @param NewSaveName 
	UFUNCTION(BlueprintCallable)
	void NewSaveButtonPressed(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel, const FString& NewSaveName);

	/// 点击新游戏的加号按下时
	/// @param LoadSlotSwitcherViewModel
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel);

	/// 加载选择的存档
	/// @param LoadSlotSwitcherViewModel 
	UFUNCTION(BlueprintCallable)
	void SelectLoadButtonPressed(UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel);

	/// 确认删除存档
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	/// 开始按钮按下
	UFUNCTION(BlueprintCallable)
	void StartButtonPressed();


	/// 从磁盘加载当前存档插槽切换器的数据(如果有)
	UFUNCTION(BlueprintCallable)
	void LoadSlotData();

	FString GetTestStr() const { return TestStr; }
	void    SetTestStr(const FString& NewTestStr) { this->TestStr = NewTestStr; }

protected:
	/// 用于生成LoadSlotSwitcher的ViewModel类
	UPROPERTY(EditDefaultsOnly, Category="MVVMViewModel")
	TSubclassOf<UMVVMLoadSlotSwitcher> UmvvmLoadSlotSwitcherClass;

	ARPGAuraGameModeBase* GetMyGameMode();

	UBaseGameInstance* GetMyGameInstance();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(Allowprivateaccess="true"))
	FString TestStr{};

	/// 加载切换器槽位所对应的的MVVM-ViewModel
	UPROPERTY()
	TMap<FGameplayTag, UMVVMLoadSlotSwitcher*> LoadSlotSwitcherViewModelMap;

	UPROPERTY()
	TObjectPtr<ARPGAuraGameModeBase> MyGameMode;

	UPROPERTY()
	TObjectPtr<UBaseGameInstance> MyGameInstance;

	/// 玩家当前正在选择的插槽切换器所对应的ViewModel
	UPROPERTY()
	TObjectPtr<UMVVMLoadSlotSwitcher> CurrentSelectedSlotSwitcher;
};
