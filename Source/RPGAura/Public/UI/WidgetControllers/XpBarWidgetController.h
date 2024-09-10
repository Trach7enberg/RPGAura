// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "XpBarWidgetController.generated.h"

/**
 *  经验条Widget的控制器
 */
UCLASS()
class RPGAURA_API UXpBarWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;

	/// 当角色的Xp值改变时,该委托会广播计算过的xp的百分比值
	UPROPERTY(BlueprintAssignable,Category="XP")
	FOnGetXpSignature OnGetXp;

	

private:
	/// 根据角色当前的经验值获得经验条的百分比
	UFUNCTION()
	void CalculateXpPercent(int32 Xp);
};
