// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/BaseHUD.h"
#include "LoadScreenHUD.generated.h"

class UMVVMLoadScreen;
class ULoadScreenWidget;
/**
 *  "加载地图"Level 所使用的HUD
 */
UCLASS()
class RPGAURA_API ALoadScreenHUD : public ABaseHUD
{
	GENERATED_BODY()

public:
	ALoadScreenHUD();
	virtual void BeginPlay() override;

protected:
	/// 加载界面widget的生成类class
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Widget")
	TSubclassOf<ULoadScreenWidget> LoadScreenWidgetClass;

	/// 加载界面的ViewModelClass
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="MVVMViewModel")
	TSubclassOf<UMVVMLoadScreen> MvvmLoadScreenClass;

	/// 加载界面的widget
	UPROPERTY(BlueprintReadOnly,Category="Widget")
	TObjectPtr<ULoadScreenWidget> CurrentLoadScreenWidget;

	/// 加载界面的MVVMViewModel
	UPROPERTY(BlueprintReadOnly,Category="MVVMViewModel")
	TObjectPtr<UMVVMLoadScreen> CurrentLoadScreenViewModel;
};
