// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "AuraCharacter.generated.h"

class UGameplayEffect;
class USpringArmComponent;
class UCameraComponent;

/**
 * Aura人物类
 */
UCLASS()
class RPGAURA_API AAuraCharacter : public ACharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual UAbilitySystemComponent *GetAbilitySystemComponent() const override;


	/// 服务器调用的函数,在此之前已经设置好玩家的控制器
	/// @param NewController 
	virtual void PossessedBy(AController *NewController) override;

	/// 客户端调用的函数,当PlayerState已经被复制
	virtual void OnRep_PlayerState() override;

	/// 初始化HUD的主要widget
	void InitHUD() const;

	// ~ ICombatInterface
	virtual int32 GetCharacterLevel() override;
	// ~ ICombatInterface

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	UCameraComponent *CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	USpringArmComponent *SpringArmComponent;

	// 用于初始化角色重要(Vital)属性的GE类,在SecondPrimary属性初始化后执行
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="GAS | Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesGameplayEffect = nullptr;
	

	/// 初始化当前ACS的能力组件、属性集和ACS的AbilityActorInfo
	virtual void InitAbilityActorInfo() override;

	

};
