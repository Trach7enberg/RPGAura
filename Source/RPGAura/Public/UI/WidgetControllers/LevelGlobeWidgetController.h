// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "LevelGlobeWidgetController.generated.h"

/**
 * 等级球widget的控制器
 */
UCLASS()
class RPGAURA_API ULevelGlobeWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()
public:
	/// 当角色的等级改变时,该委托会广播等级
	UPROPERTY(BlueprintAssignable,Category="XP|Level")
	FOnIntegerChangeSignature OnLevelChange;
	
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;
};
