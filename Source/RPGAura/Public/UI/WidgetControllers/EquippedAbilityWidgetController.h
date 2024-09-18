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
	virtual  void BroadcastInitialValues() override;
};
