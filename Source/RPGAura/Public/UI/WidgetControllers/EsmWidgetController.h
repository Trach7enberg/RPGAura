// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "EsmWidgetController.generated.h"

/**
 * 法术管理界面UI的widget控制器
 */
UCLASS()
class RPGAURA_API UEsmWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;
};
