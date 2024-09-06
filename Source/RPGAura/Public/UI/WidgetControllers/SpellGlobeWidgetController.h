// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetControllers/BaseWidgetController.h"
#include "SpellGlobeWidgetController.generated.h"

/**
 * 
 */
UCLASS()
class RPGAURA_API USpellGlobeWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallBack() override;
	virtual void BroadcastInitialValues() override;
};