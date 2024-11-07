// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "MVVMLoadScreen.generated.h"

class UMVVMLoadSlotSwitcher;
/**
 * 用于加载屏幕的MVVM架构的ViewModel
 */
UCLASS()
class RPGAURA_API UMVVMLoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	
	void InitialLoadSlotSwitcherWidgetViewModel();

	/// 通过widget标签获取SlotSwitcher的ViewModel
	/// @param LoadSlotSwitchWidgetTag 
	/// @return 
	UFUNCTION(BlueprintCallable)
	UMVVMLoadSlotSwitcher* GetSwitcherViewModelByTag(FGameplayTag LoadSlotSwitchWidgetTag);
	
	UFUNCTION(BlueprintCallable)
	void NewSaveButtonPressed(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel,const FString& NewSaveName);

	/// 点击新游戏的加号按下时
	/// @param LoadSlotSwitcherViewModel
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel);

	
	UFUNCTION(BlueprintCallable)
	void SelectLoadButtonPressed(const UMVVMLoadSlotSwitcher* LoadSlotSwitcherViewModel,const FString& NewSaveName);
	
	FString GetTestStr() const { return TestStr; }
	void SetTestStr(const FString& NewTestStr) { this->TestStr = NewTestStr; }

	
protected:
	/// 用于生成LoadSlotSwitcher的ViewModel类
	UPROPERTY(EditDefaultsOnly, Category="MVVMViewModel")
	TSubclassOf<UMVVMLoadSlotSwitcher> UmvvmLoadSlotSwitcherClass;
	
private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(Allowprivateaccess="true"))
	FString TestStr;
	
	/// 加载槽位所对应的的MVVM-ViewModel
	UPROPERTY()
	TMap<FGameplayTag, UMVVMLoadSlotSwitcher*> LoadSlotSwitcherViewModelMap;
};
