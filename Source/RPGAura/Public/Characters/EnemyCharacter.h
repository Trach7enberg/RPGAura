// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/HighLightInterface.h"
#include "EnemyCharacter.generated.h"

class ABaseAIController;
class UBehaviorTree;
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
	virtual int32 GetCharacterLevel() override { return CharacterLevel; };
	virtual void Die() override;
	virtual AActor* GetCombatTarget() override;
	virtual void SetCombatTarget(AActor* NewCombatTarget) override;
	// ~ ICombatInterface

	virtual void PossessedBy(AController* NewController) override;
	// 属性变更委托
	UPROPERTY(BlueprintAssignable)
	FOnVitalAttributeChangedSignature OnCurrentHealthAttributeChanged;

	UPROPERTY(BlueprintAssignable)
	FOnVitalAttributeChangedSignature OnMaxHealthAttributeChanged;

	/// 仅广播血条值一次,用于初始化血条
	UFUNCTION(BlueprintCallable, Category = "Widget")
	void BroadCastHealthBarInit() const;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	/// 血条widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Widget")
	TObjectPtr<UWidgetComponent> EnemyHealthBar;

	UPROPERTY(EditAnywhere, Category ="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	/// 当前角色的攻击目标
	UPROPERTY()
	TWeakObjectPtr<AActor> CombatTarget = nullptr;

private:
	// ~ IHighLightInterface Begin
	UFUNCTION()
	void OnMouseOver(AActor* TouchedActor);


	UFUNCTION()
	void EndMouseOver(AActor* TouchedActor);
	// ~IHighLightInterface End
	
	virtual void OnGrantedTag_DeBuffStun(const FGameplayTag Tag, int32 NewTagCount) override;

	UPROPERTY()
	TObjectPtr<ABaseAIController> CurrentAiController;
};
