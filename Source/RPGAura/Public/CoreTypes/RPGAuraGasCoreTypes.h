#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "RPGAuraCoreTypes.h"
#include "RPGAuraGasCoreTypes.generated.h"

/**
 * 自定义GE上下文类
 */
USTRUCT(BlueprintType)
struct FRPGAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override { return StaticStruct(); }

	/// 序列化,将当前结构变成0和1组成的数据然后通过网络传输
	/// @param Ar 存储序列化数据或者序列化事物的工具,把数据序列化为一系列的位,内部重载了<<运算符用于序列化或者反序列化
	/// (字节序是多字节数据在计算机内存中存储或者网络传输时各字节的存储顺序 , 这里采用(Byte order neutral = Big Endian)大端字节序)
	/// @param Map 将对象映射到索引的工具,在序列化所有对象时,结构体的所有变量都被转换为字符串(由零和一组成的字符串数组),当序列开始的时候,一堆东西并排放在一起时,知道一个对象是从哪里开始的索引非常有用
	/// @param bOutSuccess 如果序列化成功,会返回true
	/// @return 
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;

	/// 创建此上下文的副本，用于复制以备以后修改
	/// @return 当前GE上下文的副本
	virtual FRPGAuraGameplayEffectContext* Duplicate() const override
	{
		FRPGAuraGameplayEffectContext* NewContext = new FRPGAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	bool IsBlockedHit() const { return bIsBlockedHit; }
	void SetBIsBlockedHit(const bool NewBIsBlockedHit) { this->bIsBlockedHit = NewBIsBlockedHit; }
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetBIsCriticalHit(const bool NewBIsCriticalHit) { this->bIsCriticalHit = NewBIsCriticalHit; }

	void SetImpulse(const FVector& NewImpulse) { Impulse = NewImpulse; }
	FVector GetImpulse() const { return Impulse; }

	/// 添加伤害类型
	/// @param DamageTag 
	void AddDamageType(const FGameplayTag& DamageTag) { DamageTypes.AddTag(DamageTag); }

	/// 获取伤害类型容器
	/// @return 
	FGameplayTagContainer GetDamageTypes() const { return DamageTypes; }

	/// 
	/// @param DeBuffInfo 
	FORCEINLINE void AddDeBuffInfo(const FDeBuffInfo& DeBuffInfo) { DeBuffInfos.Add(DeBuffInfo); }

	FORCEINLINE TArray<FDeBuffInfo> GetDeBuffInfos() { return DeBuffInfos; }

	/// 当前的GE是否是DeBuff引起的
	/// @return 
	bool IsDeBuffSideEffect() const { return bIsDeBuffSideEffect; }
	void SetIsDeBuffSideEffect(const bool IsDeBuffSideEffect) { this->bIsDeBuffSideEffect = IsDeBuffSideEffect; }

	/// 当前GE的能力是否触发击退
	/// @return 
	bool IsKnockBackHit() const { return bIsKnockBackHit; }
	void SetIsKnockBackHit(const bool IsKnockBackHit) { this->bIsKnockBackHit = IsKnockBackHit; }

	float GetRadiusDamageFallOffFactor() const { return RadiusDamageFallOffFactor; }

	void SetRadiusDamageFallOffFactor(const float NewRadiusDamageFallOffFactor)
	{
		this->RadiusDamageFallOffFactor = NewRadiusDamageFallOffFactor;
	}

protected:
	// 攻击是否被格挡
	UPROPERTY()
	bool bIsBlockedHit = false;

	// 攻击是否暴击
	UPROPERTY()
	bool bIsCriticalHit = false;

	// 当前的GE是否是DeBuff引起的
	UPROPERTY()
	bool bIsDeBuffSideEffect = false;

	// 当前GE的能力是否触发击退
	UPROPERTY()
	bool bIsKnockBackHit = false;

	// 伤害范围半径的衰减系数 (不为1则说明是范围衰减伤害)
	UPROPERTY()
	float RadiusDamageFallOffFactor = 1.f;

	// TODO 结构体需要换成弱指针包裹?
	// 当前GE拥有的DeBuff
	UPROPERTY()
	TArray<FDeBuffInfo> DeBuffInfos = TArray<FDeBuffInfo>();

	// 当前GE拥有的伤害类型
	UPROPERTY()
	FGameplayTagContainer DamageTypes;

	// 冲击向量
	UPROPERTY()
	FVector Impulse = FVector::ZeroVector;
};

// 结构体特征选项结构
template <>
struct TStructOpsTypeTraits<
		FRPGAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRPGAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
