// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "BaseDamageAbility.generated.h"

struct FMontageWithTag;
/**
 *  基础伤害类型的能力
 */
UCLASS()
class RPGAURA_API UBaseDamageAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	/// 造成伤害(给对象应用GE,可以给近战对象使用)
	/// @param Suffer
	/// @param EffectParams
	UFUNCTION(BlueprintCallable, Category="GameplayEffect")
	void CauseDamage(AActor* Suffer, UPARAM(REF)FDamageEffectParams& EffectParams);


	/// 获取随机的攻击动画
	/// @param MontageWithTags 
	/// @return 
	UFUNCTION(BlueprintCallable, Category="Combat")
	FMontageWithTag GetRandomAttackAnim(const TArray<FMontageWithTag> MontageWithTags);

protected:
	/// 技能产生的物体最大值
	UPROPERTY()
	int32 MaxAbilityCount = 1;
	
	// 当前能力造成DeBuff的几率
	UPROPERTY(EditDefauLtsOnLy, Category ="DamageDeBuff", meta=(ClampMin="0", ClampMax="100"))
	float DeBuffChance = 20.f;

	// 当前能力造成击退的几率
	UPROPERTY(EditDefauLtsOnLy, Category ="DamageDeBuff", meta=(ClampMin="0", ClampMax="100"))
	float KnockBackChance = 0.f;

	// 击退的力度
	UPROPERTY(EditDefauLtsOnLy, Category ="DamageDeBuff", meta=(ClampMin="100", ClampMax="9000"))
	float KnockBackFactor = 300.f;

	// 击退的力度方向,eg. 1为正方向, -1为反方向
	UPROPERTY(EditDefauLtsOnLy, Category ="DamageDeBuff", meta=(ClampMin=-1, ClampMax=1))
	float KnockBackDirection = 1.f;

	// 当前能力触发DeBuff时造成的伤害
	UPROPERTY(EditDefauLtsOnLy, Category ="DamageDeBuff")
	float DeBuffDamage = 5.f;

	// 当前能力触发DeBuff时,每秒伤害的间隔
	UPROPERTY(EditDefauLtsOnLy, Category ="DamageDeBuff")
	float DeBuffFrequency = 1.f;

	// DeBuff持续时间
	UPROPERTY(EditDefauLtsOnLy, Category ="DamageDeBuff")
	float DeBuffDuration = 5.f;

	// 伤害GE实体类
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GameplayEffect")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	// 当前能力所造成的伤害类型,GT映射到对应的曲线表
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypesMap;

	/// 分配一个键值对给SetByCaller,并且将当前的伤害类型传递到GE上下中
	/// @param SpecHandle 
	/// @param AbilityLevel
	void AssignTagSetByCallerMagnitudeWithDamageType(const FGameplayEffectSpecHandle& SpecHandle,
	                                                 const float AbilityLevel) const;

	/// 从DamageTypesMap里的伤害类型评估当前能力造成的基础伤害
	/// @return 
	float GetEstimatedDamageFromDamageTypesMap(int32 AbilityLevel) const;

	/// 在能力类中创建伤害GE所需要的默认参数,TargetActor需要设置否则无法生效GE(可以延迟设置)
	/// @param Params
	/// @param TargetActor 
	/// @return 
	void MakeDamageEffectParamsFromAbilityDefaults(FDamageEffectParams& Params, AActor* TargetActor = nullptr) const;

	/// 获取冲击向量
	/// @param ImpulseFactor 冲击力度缩放系数
	/// @param ImpulseAngle 冲击角度
	/// @param Direction 方向 (-1向角色的方向冲击,1则为正向冲击)
	/// @return 
	FVector GetDefaultImpulseVector(const float ImpulseFactor,const float ImpulseAngle = 45.f,const float Direction = 1.f) const;

	virtual void UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel) override;

	/// 获取技能对应的最大有效生成物体
	/// @param AbilityLevel 
	/// @return 
	virtual int32 GetValidAbilityCount(const int32 AbilityLevel);
};
