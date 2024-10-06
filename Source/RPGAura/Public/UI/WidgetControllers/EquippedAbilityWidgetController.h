// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "EquippedAbilityWidgetController.generated.h"

/**
 * 法术菜单的已装备技能widget的控制器
 */
UCLASS()
class RPGAURA_API UEquippedAbilityWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;

	/// 装备技能
	/// @param SelectedAbilityTag
	/// @param SelectedInputSlotTag 技能要装备的插槽位置(输入标签)
	UFUNCTION(BlueprintCallable)
	void EquipAbility(const FGameplayTag& SelectedAbilityTag,
	                  const FGameplayTag& SelectedInputSlotTag);

	
};
