// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "Interfaces/HighLightInterface.h"
#include "EnemyCharacter.generated.h"

class UWidgetComponent;
/**
 * 所有敌人的基类
 */
UCLASS()
class RPGAURA_API AEnemyCharacter : public ACharacterBase, public IHighLightInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	// ~ IHighLightInterface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	// ~ IHighLightInterface

	// ~ ICombatInterface
	virtual int32 GetCharacterLevel() override
	{
		return EnemyLevel;
	};
	// ~ ICombatInterface

	// 属性变更委托
	UPROPERTY(BlueprintAssignable)
	FOnVitalAttributeChangedSignature OnCurrentHealthAttributeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnVitalAttributeChangedSignature OnMaxHealthAttributeChanged;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	void BroadCastHealthBarInit() const;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;


	/// 敌人的等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EnemyLevel")
	int32 EnemyLevel = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	UWidgetComponent *EnemyHealthBar;
	
private:
	// ~ IHighLightInterface Begin
	UFUNCTION()
	void OnMouseOver(AActor *TouchedActor);


	UFUNCTION()
	void EndMouseOver(AActor *TouchedActor);
	// ~IHighLightInterface End

	

};
