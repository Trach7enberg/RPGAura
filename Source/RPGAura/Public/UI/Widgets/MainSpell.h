// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "MainSpell.generated.h"

class UVerticalBox;
class UHorizontalBox;
/**
 * MainSpell小部件
 */
UCLASS()
class RPGAURA_API UMainSpell : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	// 主动技能区域
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UHorizontalBox> OffensiveArea;

	// 被动技能区域
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UVerticalBox> PassiveArea;

	// 显示技能球的小部件实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UBaseUserWidget> SpellGlobeWidgetClass;

	/// 初始化主动技能区域
	UFUNCTION(BlueprintCallable)
	void InitOffensiveArea();

	/// 初始化被动技能区域
	UFUNCTION(BlueprintCallable)
	void InitPassiveArea();

protected:

	// 主动技能的显示(UI中)数量
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 OffensiveGlobeNum = 6;

	// 被动技能的显示(UI中)数量
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	int32 PassiveGlobeNum = 2;
	
};
