// Copyright GGBAO 

#pragma once

/**
 *	基础人物类
 */
#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "Components/TimelineComponent.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "CharacterBase.generated.h"


class UBaseAbilitySystemComponent;
class UNiagaraComponent;
class URPGAuraGameInstanceSubsystem;
class ABasePlayerState;
class UNiagaraSystem;
class UMotionWarpingComponent;
class UDamageTextComponent;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;
class UWeaponLogicBaseComponent;


UCLASS(Abstract)
class RPGAURA_API ACharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

	/// 高亮当前角色
	virtual void HighLight();

	/// 解除高亮当前角色
	virtual void UnHighLight();

	virtual void LifeSpanExpired() override;

	/// 获取当前角色的属性集 (该属性集在InitAbilityActorInfo中初始化)
	virtual UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	// ~ IAbilitySystemInterface

	/// 获取当前角色的能力组件
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// ~ ICombatInterface
	virtual int32 GetCharacterLevel() override { return 0; };
	virtual ECharacterClass GetCharacterClass() override;

	virtual FVector GetCombatSocketLocation(const FGameplayTag& GameplayTag) override;
	virtual void UpdateCharacterFacingTarget(const FVector& TargetLoc) override;
	virtual UAnimMontage* GetHitReactAnim() override;
	virtual UAnimMontage* GetDeathAnim() override;
	virtual TArray<FMontageWithTag> GetAttackAnims() override;
	virtual UAnimMontage* GetSummonAnim() override;
	virtual int32 GetMaxSummonsCount() override;
	virtual int32 GetCurrentSummonsCount() override;
	virtual void UpdateCurrentSummonsCount(int32 NewCount) override;
	virtual AActor* GetCombatTarget() override;
	virtual void SetCombatTarget(AActor* CombatTarget) override;
	/// 角色死亡 , 只在服务器上调用
	virtual void Die() override;
	virtual bool IsCharacterDie() override;
	/// 在角色头顶显示伤害
	/// 对于在服务器控制的角色将会在服务器上执行,对于客户端控制的角色将在服务器上调用这个函数然后客户端执行,无论怎么样确保显示
	/// @param Damage 需要显示的伤害
	/// @param bBlockedHit
	/// @param bCriticalHit
	UFUNCTION(NetMulticast, Reliable)
	virtual void ShowDamageNumber(const float Damage, bool bBlockedHit = false, bool bCriticalHit = false) override;
	virtual UNiagaraSystem* GetBloodEffect() override;
	virtual void StartSummonAnim() override;
	virtual void ShowDeBuffVfx(FGameplayTag DeBuffType) override;
	UFUNCTION(NetMulticast, Reliable)
	virtual void AddDeathImpulse(const FVector& Impulse) override;
	virtual void AddKnockBack(const FVector& Direction) override;
	virtual void SetCastShockAnimState(const bool Enabled) override;
	virtual bool GetCastShockAnimState() override;
	virtual USkeletalMeshComponent* GetWeaponMesh() override;
	// ~ ICombatInterface


protected:
	virtual void BeginPlay() override;

	// 当前角色的lifeSpan
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	float SelfLifeSpan;

	/// 当前角色是否在进行被击中逻辑
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	bool BIsHitReacting;

	/// 当前角色的最大移动速度
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	float MaxWalkingSpeed;

	// 角色等级
	UPROPERTY(BlueprintReadOnly, Category="Combat")
	float CharacterLevel;

	/// DeBuff特效的大小
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX", meta=(AllowPreserveRatio))
	FVector DeBuffVfxScale = FVector(1.0f);

	// 当前角色能拥有召唤物的最大数量
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat")
	int32 MaxSummonsCount;

	/// 角色添加冲击时,Mesh的冲击系数
	UPROPERTY()
	int32 ImpulseFactorMesh = 6000;

	/// 角色添加冲击时,武器Mesh的冲击系数
	UPROPERTY()
	int32 ImpulseFactorWeaponMesh = ImpulseFactorMesh / 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	FName AttackSocketName_BodyTip = "TipSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	FName AttackSocketName_LeftHand = "LeftHandAttackSocket";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	FName AttackSocketName_RightHand = "RightHandAttackSocket";

	// 当前角色的职业类别
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	ECharacterClass CharacterClass;

	/// 动画蒙太奇中的运动扭曲的WarpTargetName
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	FName WarpTargetName = "FacingTarget";

	// 播放特效的Niagara组件,(目前用于LevelUp、DeBuff)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	TObjectPtr<UNiagaraComponent> NiagaraComponent;

	/// DeBuff类型到对应的VFX
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="VFX")
	TMap<FGameplayTag, TObjectPtr<UNiagaraSystem>> DeBuffVfxMap;

	/// 标签到对应的攻击动画蒙太奇数组
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Combat")
	TArray<FMontageWithTag> AttackMontageWithTagArray;

	// 武器逻辑组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UWeaponLogicBaseComponent> WeaponLogicBaseComponent;

	/// GAS的能力组件
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/// GAS的属性集
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	// 角色的受击动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactAnimMontage;

	// 角色的死亡动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UAnimMontage> DeathAnimMontage;

	// 角色的召唤技能动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TObjectPtr<UAnimMontage> SummonAnimMontage;

	// 角色溶解材质
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MaterialInstance")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstanceCharacter;

	// 武器溶解材质
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MaterialInstance")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstanceWeapon;

	// 溶解时间线所需要的角色溶解曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UCurveFloat> DissolveFloatCurve;

	// 角色被召唤时的抖动曲线
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	TObjectPtr<UCurveFloat> SummonFloatCurve;

	/// 显示伤害的UI组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Widget")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Animation")
	UMotionWarpingComponent* MotionWarpingComponent;

	/// 当前人物被东西击中时的血液效果
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> BloodEffect;

	/// 升级时的特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	TObjectPtr<UNiagaraSystem> LevelUpEffect;

	/// 角色死亡时的声音
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<USoundBase> DeathSound;


	// 用于角色被召唤时动画的时间线组件
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Combat")
	TObjectPtr<UTimelineComponent> SummonTimelineComponent;


	/// 给角色授予能力
	void AddCharacterAbilities();

	/// 接受一个GE用来初始化角色身上的属性,次要属性必须得在主要属性初始化之后
	/// @param AttributesGameplayEffect GE类
	/// @param Level 应用GE的等级
	virtual void InitAttributes(TSubclassOf<UGameplayEffect> AttributesGameplayEffect, float Level = 1.f) const;

	/// 初始化角色身上的所有属性(职业对应的默认主要属性以及次要、vital属性),NPC和玩家的次要属性有所区分,不是同一种
	/// @param BIsPlayer 是否是玩家
	virtual void InitAllAttributes(bool BIsPlayer = false);

	/// 当前角色能否被高亮
	/// @return 能高亮则返回true
	virtual bool CanHighLight();

	/// 初始化能力系统的ActorInfo
	virtual void InitAbilityActorInfo();

	/// 向能力组件中注册当某个标签被移除或者添加时候的回调,需要在InitAbilityActorInfo之后执行
	virtual void RegisterGameplayTagEvent();

	/// 获取PlayerState(类型为项目自定义的)
	/// @return 
	virtual ABasePlayerState* GetMyPlayerState();

	/// 获取GameInstance的子系统
	/// @return 
	virtual URPGAuraGameInstanceSubsystem* GetMyGiSubSystem();

	/// 多播RPC,服务器和客户端都会调用,用于处理角色死亡
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
	/// 多播RPC,用于处理服务器和客户端的特效,当同时播放多个特效时可以使用单次使用选项
	/// @param Vfx
	/// @param VfxTransform
	/// @param LocationType
	/// @param SingleUse 
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastVfx(UNiagaraSystem* Vfx, FTransform VfxTransform,
							  EAttachLocation::Type LocationType = EAttachLocation::Type::KeepRelativeOffset,
							  bool SingleUse = false);

	/// 多播进行停止特效的播放
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastStopVfx();
private:
	/// 当前角色是否死亡
	bool bIsDie;

	// 溶解材质实例蓝图里的参数名字
	FName ScalarParam = "Dissolve";

	// 当前角色能拥有召唤物的数量
	int32 CurrentSummonsCount;

	UPROPERTY()
	TObjectPtr<ABasePlayerState> BasePlayerState;

	UPROPERTY()
	TObjectPtr<URPGAuraGameInstanceSubsystem> RPGAuraGameInstanceSubsystem;

	// 时间线组件
	UPROPERTY()
	TObjectPtr<UTimelineComponent> DissolveTimelineComponent;

	/// 角色的动态溶解材质实例
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstanceDynamic_Character;

	/// 武器的动态溶解材质实例
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstanceDynamic_Weapon;

	/// 当ASC 被授予或者被完全移除HitReact标签时的回调函数
	/// @param Tag 指定标签被移除或者被添加的标签
	/// @param NewTagCount 当前标签被移除或者被添加 多个同样的签标的计数时 (可以同时有相同类型的标签)
	UFUNCTION()
	virtual void OnGrantedTag_HitReact(const FGameplayTag Tag, int32 NewTagCount);

	/// 当ASC 被授予或者被完全移除DeBuff相关标签时的回调函数
	/// @param Tag 
	/// @param NewTagCount 
	UFUNCTION()
	virtual void OnGrantedTag_DeBuff(const FGameplayTag Tag, int32 NewTagCount);

	/*----------------
		溶解时间线
	----------------*/

	// 给指定mesh设置溶解材质
	void SetDissolveMaterial();

	/// 设置动态材质实例上的参数
	/// @param MaterialInstance 材质实例
	/// @param Value 值
	/// @param ParameterName 参数名
	void SetScalarParameterValue(
		UMaterialInstanceDynamic* MaterialInstance, const float Value, const FName ParameterName);

	// 初始化溶解时间线组件
	void InitDissolveTimeLine();

	// 开始溶解时间线
	void StartDissolveTimeline();

	// 时间线update时调用的函数
	UFUNCTION()
	void DissolveTimelineUpdateFunc(float Output);

	// 时间线完成时调用的函数
	UFUNCTION()
	void DissolveTimelineFinishedFunc();

	/*----------------
		召唤时间线
	----------------*/

	// 初始化召唤时间线组件
	void InitSummonTimeLine();

	// 开始召唤时间线动画
	void StartSummonTimeline() const;


	// 召唤时间线update时调用的函数
	UFUNCTION()
	void SummonTimelineUpdateFunc(float Output);

	// 召唤时间线完成时调用的函数
	UFUNCTION()
	void SummonTimelineFinishedFunc();
	
};
