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
	/// 广播 数据表中的表行,表行的类型为FUIWidgetRow
	UPROPERTY(BlueprintAssignable, Category="GAS | Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRow;

	/// 绑定回调到GAS系统
	virtual void BindCallBack() override;
	
	/// 广播初始值,以便UI做出相应的响应
	virtual void BroadcastInitialValues() override;
	

protected:

	/// 在GAS当中GE应用到玩家身上,并且获取资产标签时触发的回调函数
	/// @param AssetTags 
	void OnGetAssetTags(const FGameplayTagContainer &AssetTags);

	

};
