// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "TextValueWidgetController.generated.h"



struct FGameplayAttribute;
class UBaseAttributeSet;
class UAttributeInfo;
/**
 *  TextValueRow Widget的控制器 
 */
UCLASS()
class RPGAURA_API UTextValueWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	/// 可分配的属性点变化时候的委托
	UPROPERTY(BlueprintAssignable)
	FOnIntegerChangeSignature OnAssignableAttributePointsChange;

	/// 可分配的法术点变化时候的委托
	UPROPERTY(BlueprintAssignable)
	FOnIntegerChangeSignature OnAssignableSpellPointsChange;
	
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;

	/// 消耗(可分配的属性)点数,一次只能消耗一点
	UFUNCTION(BlueprintCallable)
	void CostPoint(const FGameplayTag& AttributeTag);

	/// 消耗可分配法术点数
	/// @param AbilityTag 
	UFUNCTION(BlueprintCallable)
	void SpendSpellPoint(const FGameplayTag& AbilityTag);

private:
	/// 升级对应的属性
	/// @param AttributeTag
	/// @return 
	UFUNCTION(BlueprintCallable)
	bool UpgradeAttribute(const FGameplayTag& AttributeTag); 
};
