#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
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

protected:
	
	// 攻击是否被格挡
	UPROPERTY()
	bool bIsBlockedHit  = false;

	// 攻击是否暴击
	UPROPERTY()
	bool bIsCriticalHit  = false;
};

// 结构体特征选项结构
template<>
struct TStructOpsTypeTraits< FRPGAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		 
	};
};
