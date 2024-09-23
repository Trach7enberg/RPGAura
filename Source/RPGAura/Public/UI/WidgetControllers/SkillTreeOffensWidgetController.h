// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "SkillTreeOffensWidgetController.generated.h"

/**
 * 主动技能树Widget控制器
 */
UCLASS()
class RPGAURA_API USkillTreeOffensWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;
	
};
