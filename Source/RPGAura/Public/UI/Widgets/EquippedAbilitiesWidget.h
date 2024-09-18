// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/BaseUserWidget.h"
#include "EquippedAbilitiesWidget.generated.h"

class UVerticalBox;
class UHorizontalBox;
/**
 *  法术菜单的已装备技能widget
 */
UCLASS()
class RPGAURA_API UEquippedAbilitiesWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	/// 初始化装备技能区域的主动技能栏
	UFUNCTION(BlueprintCallable)
	void InitOffensiveEquippedArea();

	/// 初始化装备技能区域的被动技能栏
	UFUNCTION(BlueprintCallable)
	void InitPassiveEquippedArea();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Equipped Abilities|Comp", meta=(BindWidget))
	TObjectPtr<UHorizontalBox> OffensiveEquippedArea;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Equipped Abilities|Comp", meta=(BindWidget))
	TObjectPtr<UVerticalBox> PassiveEquippedArea;

protected:
	/// 装备的主动技能球显示数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Equipped Abilities|Var")
	int32 EquippedAbilityNum_Offensive = 6;

	/// 装备的被动技能球显示数量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Equipped Abilities|Var")
	int32 EquippedAbilityNum_Passive = 2;

	/// EquippedSpell实体类
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
	TSubclassOf<UBaseUserWidget> EquippedSpell;

private:
	
	
};
