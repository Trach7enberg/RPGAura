// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextComponent.generated.h"

/**
 * 伤害文本widget组件
 */
UCLASS()
class RPGAURA_API UDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UDamageTextComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DamageTextComponentPropp|Text")
	FText HitMessage_Critical = FText::FromString(FString(UTF16_TO_TCHAR(TCHAR_TO_UTF16(TEXT("暴击")))));
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DamageTextComponentPropp|Text")
	FText HitMessage_Blocked = FText::FromString(FString(UTF16_TO_TCHAR(TCHAR_TO_UTF16(TEXT("格挡")))));

	/// 设置伤害文本
	/// @param DamageText 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(const FText &  DamageText);

	/// 设置击中敌人的消息文本(比如暴击、格挡)
	/// @param MessageText 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHitMessageText(const FText &  MessageText);

	/// 设置伤害文本的颜色
	/// @param NewColor 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageTextColor(FLinearColor NewColor);

	/// 设置击中敌人的消息文本颜色
	/// @param NewColor 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetHitMessageTextColor(FLinearColor NewColor);
};
