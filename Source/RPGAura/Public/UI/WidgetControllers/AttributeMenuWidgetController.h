// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


class URPGAuraGameInstanceSubsystem;
struct FGameplayAttribute;
class UBaseAttributeSet;
class UAttributeInfo;
/**
 *  属性菜单的控制器类
 */
UCLASS()
class RPGAURA_API UAttributeMenuWidgetController : public UBaseWidgetController
{
    GENERATED_BODY()

public:
    
    virtual void BindCallBack() override;
    virtual void BroadcastInitialValues() override;

	TObjectPtr<UAttributeInfo> GetAttributeInfo() const
	{
		return AttributeInfo;
	}

	void SetAttributeInfo(const TObjectPtr<UAttributeInfo>& NewAttributeInfo)
	{
		this->AttributeInfo = NewAttributeInfo;
	}

protected:
	/// 游戏标签到属性的映射数据资产表信息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:

	/// 绑定当属性集中的主要、次要属性变化时调用的回调函数
	/// @param MyAs 
	/// @param GameplayTag
	/// @param Attribute
	/// @param Subsystem
	void HandleAnyAttributeChange(const URPGAuraGameInstanceSubsystem* Subsystem,const UBaseAttributeSet* MyAs, const FGameplayTag& GameplayTag, const FGameplayAttribute& Attribute) const;

	void BroadcastAttributeInfoStruct(const URPGAuraGameInstanceSubsystem* Subsystem,const UBaseAttributeSet* MyAs, const FGameplayTag& GameplayTag, const FGameplayAttribute& Attribute) const;
};