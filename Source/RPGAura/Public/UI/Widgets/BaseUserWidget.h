// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UBaseWidgetController;
/**
 *  基础用户UI类
 */
UCLASS()
class RPGAURA_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FGameplayTag GetCurrentGameplayTag() const { return CurrentGameplayTag; }

	void SetCurrentGameplayTag(const FGameplayTag& NewGameplayTag) { this->CurrentGameplayTag = NewGameplayTag; }

	UFUNCTION(BlueprintCallable, Category="WidgetController")
	UObject* GetWidgetController() const { return WidgetController; }


	template <typename T>
	UFUNCTION(BlueprintCallable, Category="WidgetController")
	T* GetWidgetController() const { return Cast<T>(WidgetController); }

	UFUNCTION(BlueprintCallable, Category="WidgetController")
	void SetWidgetController(UObject* NewWidgetController);

protected:
	virtual void NativeOnInitialized() override;

	/// 当控制器被设置时调用该事件
	UFUNCTION(BlueprintImplementableEvent, Category="Widget|WidgetController")
	void WidgetControllerSet();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widget|WidgetGameplayTag")
	FGameplayTag CurrentGameplayTag;

private:
	UPROPERTY()
	TObjectPtr<UObject> WidgetController;
};
