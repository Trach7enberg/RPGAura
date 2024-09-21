// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "SkillTreeOffensiveWidget.generated.h"

class UTexture2D;
class UVerticalBox;
class UImage;
/**
 * 主动技能树Widget
 */
UCLASS()
class RPGAURA_API USkillTreeOffensiveWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UVerticalBox> FireSpellArea;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UVerticalBox> LightingArea;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta=(BindWidget))
	TObjectPtr<UVerticalBox> ArcaneArea;

	/// 初始化技能树的技能区域
	UFUNCTION(BlueprintCallable)
	void InitSkillTreeArea();
protected:
	/// 每一列技能树拥有技能球的个数
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 RowNum = 3;
	
	/// 用于生成的主动技能树里的技能按钮
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Class")
	TSubclassOf<UBaseUserWidget> SpellButtonClass;

	/// 连接技能树里的技能方向图片
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Class")
	TObjectPtr<UTexture2D> SkillDirectionTexture;

private:
	void CreateTreeAreaWidget(UPanelWidget* Area, const FGameplayTagContainer& WidgetTags) const;
};
