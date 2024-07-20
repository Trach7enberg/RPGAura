// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPGAURA_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/// 初始化ASC中的一些设置、比如绑定回调函数
	virtual void InitSetting();

protected:
	void OnGEAppliedToSelf(UAbilitySystemComponent *AbilitySystemComponent,
	                       const FGameplayEffectSpec &GameplayEffectSpec,
	                       FActiveGameplayEffectHandle ActiveEffectHandle);
};
