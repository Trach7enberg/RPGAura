// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "SpellButtonWidgetController.generated.h"

/**
 * 法术菜单上的法术球按钮控制器
 */
UCLASS()
class RPGAURA_API USpellButtonWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	/// 当前技能按钮被按下时进行广播状态
	/// @param AbilityTag AbilityTag即是SpellButtonWidget自带的GameplayTag
	/// @param AbilityStatusTag
	/// @param AbilityType 
	UFUNCTION(BlueprintCallable)
	void BroadCastSpellGlobeButtonSelectedChange(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag, const FGameplayTag& AbilityType);
	
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;
};
