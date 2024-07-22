// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "MainWidgetController.generated.h"

class UBaseUserWidget;
struct FGameplayTag;
struct FGameplayTagContainer;
struct FOnAttributeChangeData;

/// 用于显示当角色应用GE时的一个结构体
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	// 要显示的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag Tag = FGameplayTag();

	// 要显示的消息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Message = FText();

	// 要显示的UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UBaseUserWidget> MessageWidget;

	// 要显示的图片
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D *Image = nullptr;

};

/// 给属性变化时的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangedSignature, float, NewValue,
                                             bool, /* NewValue增加还是减少,增加则为true */ BIsIncreased);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, const FUIWidgetRow, Row);

/**
 * 主要widget的控制器类
 */
UCLASS()
class RPGAURA_API UMainWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	/// 广播GAS中的生命值变化,bool为true时候表明生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChangedSignature OnHealthChangedSignature;

	/// 广播GAS中的最大生命值变化,bool为true时候表明最大生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChangedSignature OnMaxHealthChangedSignature;

	/// 广播GAS中的魔力值变化,bool为true时候表明魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChangedSignature OnManaChangedSignature;

	/// 广播GAS中的最大魔力值变化,bool为true时候表明最大魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChangedSignature OnMaxManaChangedSignature;

	/// 广播 数据表中的表行,表行的类型为FUIWidgetRow
	UPROPERTY(BlueprintAssignable, Category="GAS | Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRow;

	/// 广播初始值,以便UI做出相应的响应
	virtual void BroadcastInitialValues() override;

	/// 绑定回调到GAS系统
	virtual void BindCallBack() override;

protected:
	// 消息数据表,不同的Tag对应不同的消息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DataTable")
	TObjectPtr<UDataTable> MessageWidgetDataTable = nullptr;

	/// 在GAS当中GE应用到玩家身上,并且获取资产标签时触发的回调函数
	/// @param AssetTags 
	void OnGetAssetTags(const FGameplayTagContainer &AssetTags);

	void HealthChanged(const FOnAttributeChangeData &Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData &Data) const;
	void ManaChanged(const FOnAttributeChangeData &Data) const;
	void MaxManaChanged(const FOnAttributeChangeData &Data) const;

	/// 通过标签获得数据表中的表行
	/// @tparam T 数据表的类型
	/// @param DataTable 要查询的数据表
	/// @param Tag 标签
	/// @return T类型的数据表 
	template <typename T>
	static T *GetDataTableRowByTag(UDataTable *DataTable, const FGameplayTag &Tag)
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}

};
