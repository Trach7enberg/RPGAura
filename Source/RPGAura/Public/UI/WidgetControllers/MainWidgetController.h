// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "MainWidgetController.generated.h"

class UBaseUserWidget;
struct FGameplayTag;
struct FGameplayTagContainer;
struct FOnAttributeChangeData;



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
	FOnVitalAttributeChangedSignature OnHealthChangedSignature;

	/// 广播GAS中的最大生命值变化,bool为true时候表明最大生命在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnVitalAttributeChangedSignature OnMaxHealthChangedSignature;

	/// 广播GAS中的魔力值变化,bool为true时候表明魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnVitalAttributeChangedSignature OnManaChangedSignature;

	/// 广播GAS中的最大魔力值变化,bool为true时候表明最大魔力值在增加
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnVitalAttributeChangedSignature OnMaxManaChangedSignature;

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
