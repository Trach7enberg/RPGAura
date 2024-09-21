// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "SkillTreePassiveWidget.generated.h"

class USpacer;
class UHorizontalBox;
/**
 * 被动技能树Widget
 */
UCLASS()
class RPGAURA_API USkillTreePassiveWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> SpellArea;

	/// 初始化技能树的技能区域
	UFUNCTION(BlueprintCallable)
	void InitSkillTreeArea();
protected:
	/// 一行技能树拥有技能球的个数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 ColNum = 3;

	/// 用于生成的主动技能树里的技能按钮
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Class")
	TSubclassOf<UBaseUserWidget> SpellButtonClass;

private:
	/// 设置Spacer的slot的Size格式
	/// @param Spacer 
	/// @param Size 
	void SetSpacerSlotSize(const USpacer* Spacer,const FSlateChildSize Size) const ;
	
	void CreateTreeAreaWidget(UPanelWidget* Area, const FGameplayTagContainer& WidgetTags) const;
};
