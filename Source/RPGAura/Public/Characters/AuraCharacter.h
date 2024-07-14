// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "AuraCharacter.generated.h"

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


	virtual void PossessedBy(AController *NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	UCameraComponent *CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	USpringArmComponent *SpringArmComponent;

private:
	/// 初始化当前ACS的能力组件、属性集和ACS的AbilityActorInfo
	void InitAbilityActorInfo();

};
