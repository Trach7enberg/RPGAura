// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "MVVMLoadSlotSwitcher.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSlotSwitcherSelected, const FGameplayTag&, WidgetTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectButtonChange, const bool, Enabled);

/**
 * 用于插槽切换器Widget的ViewModel
 */
UCLASS()
class RPGAURA_API UMVVMLoadSlotSwitcher : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UMVVMLoadSlotSwitcher();

	/// 切换器委托
	UPROPERTY(BlueprintAssignable)
	FSlotSwitcherSelected OnSlotSwitcherSelected;

	/// 加载存档的选择按钮事件
	UPROPERTY(BlueprintAssignable)
	FSelectButtonChange OnSelectButtonChange;

	/// 广播插槽切换器的切换状态变化并且设置暂存当前切换器处于的Slot
	void BroadcastSwitcherLoadSlotChange(const FGameplayTag& SlotTag);

	/// 
	/// @param Enabled 
	void BroadcastSelectButtonChange(const bool& Enabled);

	/// 处于初始化LoadSlotWidgetSwitcher时的事件
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnInitializeLoadSlotSwitcherWidget();

	FString      GetTestStr() const { return TestStr; }
	FString      GetMapName() const { return MapName; }
	FString      GetPlayerName() const { return PlayerName; }
	int32        GetLoadSlotInSave() const { return LoadSlotInSave; }
	FString      GetLoadSlotName() const { return LoadSlotName; }
	int32        GetPlayerLevel() const { return PlayerLevel; }
	FName        GetPlayerStartTag() const { return PlayerStartTag; }
	FGameplayTag GetCurrentLoadSlotSwitcherLoadSlotTag() const { return CurrentLoadSlotSwitcherLoadSlot; }

	void SetTestStr(const FString& NewTestStr) { UE_MVVM_SET_PROPERTY_VALUE(TestStr, NewTestStr); }
	void SetMapName(const FString& NewMapName) { UE_MVVM_SET_PROPERTY_VALUE(MapName, NewMapName); }
	void SetLoadSlotName(const FString& NewLoadSlotName) { this->LoadSlotName = NewLoadSlotName; }
	void SetPlayerName(const FString& NewPlayerName) { UE_MVVM_SET_PROPERTY_VALUE(PlayerName, NewPlayerName); }
	void SetPlayerLevel(const int32 NewPlayerLevel) { UE_MVVM_SET_PROPERTY_VALUE(PlayerLevel, NewPlayerLevel); }
	void SetLoadSlotInSave(int32 NewLoadSlotInSave) { UE_MVVM_SET_PROPERTY_VALUE(LoadSlotInSave, NewLoadSlotInSave); }
	void SetPlayerStartTag(const FName& NewDefaultPlayerStartTag) { this->PlayerStartTag = NewDefaultPlayerStartTag; }

	void SetCurrentLoadSlotSwitcherLoadSlotTag(const FGameplayTag& NewCurrentLoadSlotSwitcherLoadSlotTag)
	{
		this->CurrentLoadSlotSwitcherLoadSlot = NewCurrentLoadSlotSwitcherLoadSlotTag;
	}

private:
	UPROPERTY()
	FString LoadSlotName{};

	/// 关卡默认的初始位置的标识
	UPROPERTY()
	FName PlayerStartTag{};

	/// 测试占位符
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(Allowprivateaccess="true"))
	FString TestStr;

	/// 加载(Taken)槽位中显示的玩家名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(Allowprivateaccess="true"))
	FString PlayerName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(Allowprivateaccess="true"))
	int32 PlayerLevel = 1;

	/// 加载(Taken)槽位中显示的地图名字
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(Allowprivateaccess="true"))
	FString MapName{};

	// 用于保存到磁盘的数据的插槽索引(和Widget小部件无关)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, meta=(Allowprivateaccess="true"))
	int32 LoadSlotInSave = 0;

	UPROPERTY()
	FGameplayTag CurrentLoadSlotSwitcherLoadSlot = FGameplayTag::EmptyTag;
};
