// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "Interfaces/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UMotionWarpingComponent;
class UGameplayEffect;
class USpringArmComponent;
class UCameraComponent;

/**
 * Aura人物类
 */
UCLASS()
class RPGAURA_API AAuraCharacter : public ACharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;


	/// 服务器调用的函数,在此之前已经设置好玩家的控制器
	/// @param NewController 
	virtual void PossessedBy(AController* NewController) override;

	/// 客户端调用的函数,当PlayerState已经被复制
	virtual void OnRep_PlayerState() override;

	/// 初始化HUD的主要widget
	void InitHUD();

	// ~ ICombatInterface
	virtual int32 GetCharacterLevel() override;

	// ~ ICombatInterface

	// ~ IPlayerInterface
	virtual void AddToPlayerXP(const int32 AddedXp) override;
	virtual int32 GetPlayerCurrentXP() override;
	virtual bool CanBeLevelUp() override;
	virtual void LevelUp() override;
	virtual int32 GetAttributePointsReward(int32 InCharacterLevel) override;
	virtual int32 GetSpellPointsReward(int32 InCharacterLevel) override;
	virtual void AddToSpellPoints(int32 Points) override;
	virtual void AddToAttributesPoints(int32 Points) override;
	virtual int32 GetCurrentAssignableAttributePoints() override;
	virtual int32 GetCurrentAssignableSpellPoints() override;
	// ~ IPlayerInterface


protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Camera")
	USpringArmComponent* SpringArmComponent;

	/// 初始化当前ACS的能力组件、属性集和ACS的AbilityActorInfo
	virtual void InitAbilityActorInfo() override;

	/// 播放升级特效,服务器和客户端
	UFUNCTION(NetMulticast, Reliable)
	virtual void MultiCastLevelVfx();

private :
	FString GetNetModeStr() const;

	virtual void OnGrantedTag_HitReact(const FGameplayTag Tag, int32 NewTagCount) override;
};
