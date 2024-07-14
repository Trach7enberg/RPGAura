// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"

/**
 * 基础属性集
 */
UCLASS()
class RPGAURA_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();

	// CurrentHealth属性
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Hp")
	FGameplayAttributeData CurrentHealth;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Hp")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMana, BlueprintReadOnly, Category = "Mp")
	FGameplayAttributeData CurrentMana;

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Mp")
	FGameplayAttributeData MaxMana;

	/// CurrentHealth变量复制到客户端的时候,就会调用该函数
	/// 该函数可以接受1个参数,但是必须也是FGameplayAttributeData类型的
	/// 当有一个参数时,它将是旧值作为参数
	UFUNCTION()
	void OnRep_CurrentHealth(const FGameplayAttributeData &OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData &OldMaxHealth) const;

	UFUNCTION()
	void OnRep_CurrentMana(const FGameplayAttributeData &OldCurrentMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData &OldMaxMana) const;

	/// 设置 当前类 中的属性在网络中如何进行复制
	/// @param OutLifetimeProps 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;
};
