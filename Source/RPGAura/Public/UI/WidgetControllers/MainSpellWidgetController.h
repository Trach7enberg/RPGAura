// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "MainSpellWidgetController.generated.h"

/**
 *  MainSpell小部件的控制器
 */
UCLASS()
class RPGAURA_API UMainSpellWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;
};
