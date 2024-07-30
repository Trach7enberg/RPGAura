// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FOnAttributeChangeData;
class UAttributeInfo;
struct FRPGAuraAttributeInfo;


/**
 *  属性菜单的控制器类
 */
UCLASS()
class RPGAURA_API UAttributeMenuWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallBack() override;

	/// 属性值变化时的委托
	UPROPERTY(BlueprintAssignable, Category="GAS | Attributes")
	FOnAttributeChangedSignature OnAttributeChanged;

	/// 游戏标签到属性的映射数据资产表信息
	/// @return 映射数据资产表信息
	TObjectPtr<UAttributeInfo> GetAttributeInfoAsset() const
	{
		return AttributeInfo;
	}

protected:
	/// 游戏标签到属性的映射数据资产表信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAttributeInfo> AttributeInfo = nullptr;

private:
	void HandleAnyAttributeChange(const FOnAttributeChangeData &Data) const;
};
