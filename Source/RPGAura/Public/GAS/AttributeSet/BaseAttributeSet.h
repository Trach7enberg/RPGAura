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

class URPGAuraGameInstanceSubsystem;
class UBaseAbilitySystemComponent;
// 不再使用下面这个委托,已经创建一个函数指针来代替了
DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);

// 返回指定类型FGameplayAttribute且无函数参数的方法指针,以F为前缀,不是T,我们指定了返回类似 所以不是模板了
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;

template <class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/// 用于存前某个GE影响当前角色属性集的相关上下文数据
USTRUCT(BlueprintType)
struct FEffectProp
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	FGameplayEffectContextHandle EffectContextHandle;

	// 发起GE的源Asc
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	UAbilitySystemComponent* SourceAsc = nullptr;

	// 发起GE的源Avatar
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AActor* SourceAvatar = nullptr;

	// 发起GE的源控制器 (如果有的话)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AController* SourceController = nullptr;

	// 发起GE的源角色 (如果有的话)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	ACharacter* SourceCharacter = nullptr;

	// 接收GE的那个目标Asc
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	UAbilitySystemComponent* TargetAsc = nullptr;

	// 接收GE的那个目标Avatar
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AActor* TargetAvatar = nullptr;

	// 接收GE的那个目标控制器 (如果有的话)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	AController* TargetController = nullptr;

	// 接收GE的那个目标角色 (如果有的话)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effect")
	ACharacter* TargetCharacter = nullptr;
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


	// TMap<FGameplayTag, FAttributeSignature> TagToAttributeMap;
	// 键为FGameplayTag , 值为一个无参数且返回类型为FGameplayAttribute的函数指针
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagToAttributeMap;


	/// 设置 当前类 中的属性在网络中如何进行复制
	/// @param OutLifetimeProps 
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Primary 属性
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


	// Second Primary属性
	UPROPERTY(ReplicatedUsing = OnRep_Armor, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Armor);

	UPROPERTY(ReplicatedUsing = OnRep_ArmorPenetration, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ArmorPenetration);

	UPROPERTY(ReplicatedUsing = OnRep_BlockChance, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, BlockChance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitChance, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitChance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitResistance, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitResistance);

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitDamage, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CriticalHitDamage);

	UPROPERTY(ReplicatedUsing = OnRep_HealthRegeneration, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, HealthRegeneration);

	UPROPERTY(ReplicatedUsing = OnRep_ManaRegeneration, BlueprintReadOnly, Category = "Second Primary Attribute")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ManaRegeneration);

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth);

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Vital Attribute")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxMana);

	// Secondary Resistance属性
	UPROPERTY(ReplicatedUsing = OnRep_FireResistance, BlueprintReadOnly,
		Category = "Second Primary Attribute|Resistance")
	FGameplayAttributeData FireResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, FireResistance);

	UPROPERTY(ReplicatedUsing = OnRep_PhysicalResistance, BlueprintReadOnly,
		Category = "Second Primary Attribute|Resistance")
	FGameplayAttributeData PhysicalResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, PhysicalResistance);

	UPROPERTY(ReplicatedUsing = OnRep_LightingResistance, BlueprintReadOnly,
		Category = "Second Primary Attribute|Resistance")
	FGameplayAttributeData LightingResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, LightingResistance);

	UPROPERTY(ReplicatedUsing = OnRep_ArcaneResistance, BlueprintReadOnly,
		Category = "Second Primary Attribute|Resistance")
	FGameplayAttributeData ArcaneResistance;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, ArcaneResistance);


	// Vital 属性
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CurrentHealth);

	UPROPERTY(ReplicatedUsing = OnRep_CurrentMana, BlueprintReadOnly, Category = "Vital Attributes")
	FGameplayAttributeData CurrentMana;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, CurrentMana);
 
	// Meta属性
	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData InComingDamage;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, InComingDamage);

	UPROPERTY(BlueprintReadOnly, Category="Meta Attributes")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, IncomingXP);

	// Primary OnRep
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldValue) const;
	// Primary OnRep

	// Secondary OnRep
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;

	// Secondary Resistance OnRep
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_LightingResistance(const FGameplayAttributeData& OldValue) const;

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const;

	/**
	 *  Vital OnRep
	 *  CurrentHealth变量复制到客户端的时候,就会调用该函数
	 *  该函数可以接受1个参数,但是必须也是FGameplayAttributeData类型的
	 *  当有一个参数时,它将是旧值作为参数
	 */
	UFUNCTION()
	void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue) const;


	UFUNCTION()
	void OnRep_CurrentMana(const FGameplayAttributeData& OldValue) const;


	/// 设置FEffectProp,该结构存储着和Data相关的GAS ActorInfo
	/// @param Data
	/// @param EffectProp 要设置的FEffectProp结构
	void UpdateCurrentGeProp(const FGameplayEffectModCallbackData& Data, FEffectProp& EffectProp);
	

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual  void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	/// 这个函数在游戏效果改变一个属性之后被执行
	/// @param Data 
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/// 获取我们定义的能力系统组件
	/// @return 能力系统组件
	UBaseAbilitySystemComponent* GetMyCurrentAbilitySystem();

private:
	/// 用于存前某个GE影响当前角色属性集的相关上下文数据
	FEffectProp EffectProperties{};

	/// 是否在升级 , 用于修复当角色升级时不会满血满蓝还剩一点才满的问题
	bool bIsLevelingUpHealth = false;
	bool bIsLevelingUpMana = false;

	UPROPERTY()
	TObjectPtr<UBaseAbilitySystemComponent> CurrentMyAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<URPGAuraGameInstanceSubsystem> GiSubSystem;

	URPGAuraGameInstanceSubsystem* GetGiSubSystem();

	/// 发送经验事件给Instigator能力类中的的WaitForGameplayEvent
	/// (玩家拥有被动能力,该能力一直在监听等待一个游戏事件,响应之后会对自身应用一个修改IncomingXP的GE)
	/// @param Sufferer 
	/// @param Instigator 玩家
	/// @return 
	bool SendXpGamePlayEvent( AActor* Sufferer, AActor* Instigator);
};
