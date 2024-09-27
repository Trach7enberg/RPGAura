// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "SkillTreePassiveWidgetController.generated.h"

/**
 * 被动技能树的widget控制器
 */
UCLASS()
class RPGAURA_API USkillTreePassiveWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;
};
