// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/// 用于存前某个GE影响当前角色属性集的相关上下文数据
USTRUCT(BlueprintType)
struct FEffectProp
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	UAbilitySystemComponent *SourceAsc = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AActor *SourceAvatar = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AController *SourceController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	ACharacter *SourceCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	UAbilitySystemComponent *TargetAsc = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AActor *TargetAvatar = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AController *TargetController = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	ACharacter *TargetCharacter = nullptr;


};

/**
 * 基础属性集
 */
UCLASS()
class RPGAURA_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBaseAttributeSet();

	/// 设置 当前类 中的属性在网络中如何进行复制
	/// @param OutLifetimeProps 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const override;

	/**
	 *  Primary 属性
	 */

	UPROPERTY(ReplicatedUsing = OnRep_Strength, BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Strength);

	UPROPERTY(ReplicatedUsing = OnRep_Intelligence, BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Intelligence);

	UPROPERTY(ReplicatedUsing = OnRep_Resilience, BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Resilience);

	UPROPERTY(ReplicatedUsing = OnRep_Vigor, BlueprintReadOnly, Category = "Primary Attribute")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Vigor);

	/**
	 *  Primary 属性
	 */

	/**
	 *  Vital 属性
	 */
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CurrentHealth);


	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMana, BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData CurrentMana;

	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CurrentMana);

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana);
	/**
	 *  Vital 属性
	 */


	/**
	 *  Primary OnRep
	 */
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData &OldValue) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData &OldValue) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData &OldValue) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData &OldValue) const;

	/**
	 *  Primary OnRep
	 */

	/**
	 *  Vital OnRep
	 *  CurrentHealth变量复制到客户端的时候,就会调用该函数
	 *  该函数可以接受1个参数,但是必须也是FGameplayAttributeData类型的
	 *  当有一个参数时,它将是旧值作为参数
	 */
	UFUNCTION()
	void OnRep_CurrentHealth(const FGameplayAttributeData &OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData &OldMaxHealth) const;

	UFUNCTION()
	void OnRep_CurrentMana(const FGameplayAttributeData &OldCurrentMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData &OldMaxMana) const;

	/**
	 *  Vital OnRep
	 */

	/// 初始化FEffectProp
	void InitCurrentGeProp(const FGameplayEffectModCallbackData &Data, FEffectProp &EffectProp);

	virtual void PreAttributeChange(const FGameplayAttribute &Attribute, float &NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute &Attribute, float &NewValue) const override;
	/// 这个函数在游戏效果改变一个属性之后被执行
	/// @param Data 
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData &Data) override;

private:
	float DefaultCurrentMana = 50.f;
	float DefaultCurrentHealth = 50.f;

	float DefaultMaxHealth = 100.f;
	float DefaultMaxMana = 100.f;

	/// 用于存前某个GE影响当前角色属性集的相关上下文数据
	FEffectProp EffectProperties{};
};
