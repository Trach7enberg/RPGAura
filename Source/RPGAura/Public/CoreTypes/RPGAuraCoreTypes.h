// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"
#include "RPGAuraCoreTypes.generated.h"

class URPGAuraGameInstanceSubsystem;
class USpellButtonWidgetController;
struct FScalableFloat;
class UGameplayAbility;
class UBaseUserWidget;
class UAttributeSet;
class UAbilitySystemComponent;

/*-------------------------------
	BaseWidgetController类使用
	Widget控制器需要的基本参数的结构体
-------------------------------*/
USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() {}

	FWidgetControllerParams(APlayerController* Pc,
	                        APlayerState* Ps,
	                        UAbilitySystemComponent* Asc,
	                        UAttributeSet* As,
	                        URPGAuraGameInstanceSubsystem* MyGiSubSystem) :
		CurrentPlayerController(Pc), CurrentPlayerState(Ps),
		CurrentAbilitySystemComponent(Asc), CurrentAttributeSet(As), GameInstanceSubsystem(MyGiSubSystem) {}

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> CurrentPlayerController = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> CurrentPlayerState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> CurrentAbilitySystemComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> CurrentAttributeSet = nullptr;

	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<URPGAuraGameInstanceSubsystem> GameInstanceSubsystem = nullptr;
};


/*-----------------------------
	UAttributeInfo类使用
	用于广播GAS属性的结构体
-----------------------------*/
USTRUCT(BlueprintType)
struct FRPGAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName = FText();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription = FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
};


/*-------------------------------------------------------
	URPGAuraGameInstanceSubsystem使用
	用于显示Message Widget的结构体,当角色拾取到什么东西时就会弹出
-------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	// 要显示的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag Tag = FGameplayTag();

	// 要显示的消息
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FText Message = FText();

	// 要显示的UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UBaseUserWidget> MessageWidget;

	// 要显示的图片
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, const FUIWidgetRow, Row);

/// 用于广播GAS属性变化的结构体,更新属性菜单的属性值显示 (包括vital、primary、secondary属性)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, const FRPGAuraAttributeInfo&, Info);


/*-----------------------------------------------------------------
	UGlobeBarWidgetController使用
	给生命值、魔力值属性变化时的委托,BIsIncreased 增加还是减少,增加则为true
-----------------------------------------------------------------*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVitalAttributeChangedSignature,
                                             float,
                                             NewValue,
                                             bool,
                                             /* BIsIncreased增加还是减少,增加则为true */
                                             BIsIncreased);

/*-------------------------------
	FRPGAuraGameplayTags结构类使用
-------------------------------*/
UENUM(BlueprintType)
enum class EGameplayTagType:uint8
{
	AllGameplayTags UMETA(DisplayName = "全部标签(All)"),
	VitalGameplayTags UMETA(DisplayName = "重要标签(Vital)"),
	PrimaryGameplayTags UMETA(DisplayName = "主要标签(Primary)"),
	SecondaryGameplayTags UMETA(DisplayName = "次要标签(Secondary)")
};


/*------------------------------------
	UCharacterClassInfo 数据资产使用
	角色职业枚举类
------------------------------------*/
UENUM(BlueprintType)
enum class ECharacterClass :uint8
{
	// 法术师
	Elementalist UMETA(DisplayName = "AI法术师"),

	// 战士
	Warrior UMETA(DisplayName = "AI战士"),

	//游侠
	Ranger UMETA(DisplayName = "AI游侠"),

	Elementalist_Player UMETA(DisplayName = "玩家法术师"),
	Warrior_Player UMETA(DisplayName = "玩家战士"),
	Ranger_Player UMETA(DisplayName = "玩家游侠")
};

/// 每个角色职业的默认属性数据
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	// 角色的主要GE属性
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DefaultAttributes|Primary")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	// 角色的主要初始能力
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DefaultAbilities")
	TArray<TSubclassOf<UGameplayAbility>> PrimaryStartUpAbilities;

	// 角色的初始伤害抗性
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DefaultAttributes|Secondary")
	TSubclassOf<UGameplayEffect> SecondaryResistanceAttributes;

	// 角色被击杀时奖励的经验
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DefaultAttributes|Secondary")
	FScalableFloat XPReward = FScalableFloat();

	// 角色的初始被动能力
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="DefaultAttributes|StartUpPassive")
	TArray<TSubclassOf<UGameplayAbility>> StartUpPassiveAbilities;
};


/*------------------------
	CombatInterface使用
------------------------*/
/// 标签对应的角色攻击蒙太奇动画
USTRUCT(BlueprintType)
struct FMontageWithTag
{
	GENERATED_BODY()

	// 动画蒙太奇
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> AnimMontage = nullptr;

	// 动画的标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGameplayTag MontageTag = FGameplayTag();

	// 与当前结构体蒙太奇对应的声音
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USoundBase> ImpactSound = nullptr;
};

/// 角色死亡的委托(死亡动画之前)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);


/*--------------------------------
	UTagToAbilityInfoAsset使用
--------------------------------*/

/// 技能信息结构体 (能力标签)映射到对应(能力)的结构体
USTRUCT(BlueprintType)
struct FTagToAbilityInfo
{
	GENERATED_BODY()

	// 与当前技能对应的标签
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	FGameplayTag AbilityTag = FGameplayTag();

	// 当前能力的分类
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	FGameplayTag AbilityType = FGameplayTag();

	// 当前技能的状态,是装备还是未解锁等等,由程序设置,手动设置暂无效果(通过能力查找)
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	FGameplayTag StatusTag = FGameplayTag();

	// 能力对应的冷却标签(如果能力使用了冷却的话)
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	FGameplayTag AbilityCoolDownTag = FGameplayTag();

	// 与当前技能相对于的输入标签,由程序设置,手动设置暂无效果(通过能力查找)
	// 手动设置仅用作显示装备栏上的按键输入名称
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	FGameplayTag InputTag = FGameplayTag();

	// 能力对应的名字
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	FText AbilityName = FText();

	// 与当前技能对应的图标
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	TObjectPtr<const UTexture2D> AbilityIcon = nullptr;

	// 与当前技能对应的材质
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	TObjectPtr<const UMaterialInterface> AbilityBackGroundMaterial = nullptr;

	// 当前技能所需要的人物等级
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	int32 LevelRequirement = 1;

	// 当前技能对应的游戏能力类
	UPROPERTY(EditDefauLtsOnLy, BLueprintReadOnLy)
	TSubclassOf<UGameplayAbility> AbilityClass = nullptr;

	/// 和输入键标签有关的成员数据是否有效
	/// @return 
	bool InfoDataInputIsValid() const
	{
		return AbilityTag.IsValid() && AbilityType.IsValid() && InputTag.IsValid() && AbilityIcon &&
			AbilityBackGroundMaterial;
	}

	/// 和能力标签有关的成员数据是否有效
	/// @return 
	bool InfoDataAbilityIsValid() const
	{
		return AbilityTag.IsValid() && AbilityType.IsValid() && StatusTag.IsValid() && AbilityIcon &&
			AbilityBackGroundMaterial;
	}
};

// 输入键与对应绑定的能力变化或初始化时的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,
                                            const FTagToAbilityInfo&, Info);

/*-------------------------
	ULevelUpInfoAsset使用
-------------------------*/
/// 相应等级所对应的升级奖励结构体信息
USTRUCT(BlueprintType)
struct FLevelUpInfoStruct
{
	GENERATED_BODY()

	FLevelUpInfoStruct() {}
	FLevelUpInfoStruct(const int32 NewLeveRequirement): LeveRequirement(NewLeveRequirement) {}

	/// 当前等级所需要的经验值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 LeveRequirement = 0;

	/// 升级到当前等级所奖励的可分配属性点数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 AttributedPointAward = 1;

	/// 升级到当前等级所奖励的可分配的法术点数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 SpellPointAward = 1;
};

/*-------------------------
	ABasePlayerState使用
-------------------------*/

/// 用于广播当人物的经验值改变时的委托,不用于蓝图绑定
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerInfoChangeSignature, int32);

/*----------------------------------------------------------
	UXpBarWidgetController、UTextValueWidgetController使用
----------------------------------------------------------*/

/// 经验值改变时的委托(经验值转换成XpBar的百分比了)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetXpSignature, float, XpPercent);

/// 属性点、技能点增加奖励的委托(任何使用int32的都可以用这个进行广播)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntegerChangeSignature, int32, Value);


/*--------------------------------------------------------------------
	URPGAuraGameInstanceSubsystem、USpellButtonWidgetController使用	
--------------------------------------------------------------------*/

/// 法术菜单的按钮发生点击变化时的委托,用于限制法术菜单中只能选中一个法术按钮球
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellButtonSelectedChange, const USpellButtonWidgetController*,
                                              SpellButtonWidgetController, const FGameplayTag&, AbilityTag,
                                              const FGameplayTag&, AbilityStatusTag, const FGameplayTag&,
                                              AbilityTypeTag);

// 装备、切换法术菜单中的技能到技能栏时
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnAbilityEquippedChange, const FGameplayTag&, AbilityTag,
                                              const FGameplayTag&, AbilityStatusTag, const FGameplayTag&, NewInputSlot,
                                              const FGameplayTag&, OldInputSlot, const bool, bIsSwapAbilitySlot);

/*---------------------------
	UBaseGameplayAbility使用
---------------------------*/

/// 描述能力详细信息的结构体
USTRUCT(BlueprintType)
struct FAbilityDescription
{
	GENERATED_BODY()

	FAbilityDescription() {}

	/// 能力标签
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Ability")
	FGameplayTag AbilityTag = FGameplayTag();

	/// 当前能力的正常描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Descriptions", meta=(MultiLine))
	FText DescriptionNormal = FText();

	/// 当前能力未解锁的描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Descriptions", meta=(MultiLine))
	FText DescriptionLocked = FText();

	/// 当前能力下一等级的描述
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Descriptions", meta=(MultiLine))
	FText DescriptionNextLevel = FText();

	/// 当前技能描述结构体是否有效
	/// @return 
	bool IsDescriptionValid() const { return AbilityTag.IsValid(); }

	/// 当前技能描述内容是否为空
	/// @return 
	bool IsDescriptionEmpty() const
	{
		return DescriptionNormal.IsEmpty() && DescriptionLocked.IsEmpty() && DescriptionNextLevel.IsEmpty();
	}
};

/*---------------------------
	UBaseDamageAbility使用
---------------------------*/

/// 给伤害能力类创建GE时所使用的参数结构
USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float DeBuffChance = 0.f;

	UPROPERTY()
	float DeBuffDamage = 0.f;

	UPROPERTY()
	float DeBuffDuration = 0.f;

	UPROPERTY()
	float DeBuffFrequency = 0.f;

	UPROPERTY()
	float KnockBackChance = 0.f;
	
	UPROPERTY()
	float KnockBackFactor = 0.f;

	UPROPERTY()
	int32 AbilityLevel = 1;

	/// 角色被击杀死亡时候的冲击点方向(一般为归一化)
	UPROPERTY()
	FVector ImpulseVector = FVector::Zero();

	UPROPERTY()
	TMap<FGameplayTag, FScalableFloat> DamageTypesMap = TMap<FGameplayTag, FScalableFloat>();

	/// 默认为发起GE的Asc的化身
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

	/// 发起GE的Actor的ASC
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;

	/// 被应用GE的Actor的ASC
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

	/// 该Params的(部分重要)参数是否有效
	/// @return 
	bool IsParamsValid() const
	{
		return SourceAbilitySystemComponent && TargetAbilitySystemComponent && DamageGameplayEffectClass &&
			DamageTypesMap.Num() > 0;
	}
};


/*------------------------------------------------------
	ExecCalcDamage、FRPGAuraGameplayEffectContext使用
------------------------------------------------------*/

/// DeBuff信息结构体
USTRUCT(BlueprintType)
struct FDeBuffInfo
{
	GENERATED_BODY()

	FDeBuffInfo() {}

	FDeBuffInfo(const FGameplayTag& DamageType, const FGameplayTag& DeBuffType, const bool bIsSuccessfulDeBuff,
	            const float DeBuffDamage, const float DeBuffDuration, const float DeBuffFrequency)
		: DamageType(DamageType),
		  DeBuffType(DeBuffType),
		  bIsSuccessfulDeBuff(bIsSuccessfulDeBuff),
		  DeBuffDamage(DeBuffDamage),
		  DeBuffDuration(DeBuffDuration),
		  DeBuffFrequency(DeBuffFrequency) {}

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	FGameplayTag DeBuffType = FGameplayTag();

	UPROPERTY()
	bool bIsSuccessfulDeBuff = false;

	UPROPERTY()
	float DeBuffDamage = 0.f;

	UPROPERTY()
	float DeBuffDuration = 0.f;

	UPROPERTY()
	float DeBuffFrequency = 0.f;

	/// 重写网络序列化
	/// @param Ar 
	/// @param Map 
	/// @param bOutSuccess
	/// @return 
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		uint16 RepBits = 0;
		if (Ar.IsSaving())
		{
			if (DamageType.IsValid()) { RepBits |= 1 << 0; }
			if (DeBuffType.IsValid()) { RepBits |= 1 << 1; }
			if (bIsSuccessfulDeBuff) { RepBits |= 1 << 2; }
		}
		Ar.SerializeBits(&RepBits, 6);

		bool bOutSuccessLocal = true;
		if (RepBits & (1 << 0))
		{
			DamageType.NetSerialize(Ar, Map, bOutSuccessLocal);
			bOutSuccess &= bOutSuccessLocal;
		}
		if (RepBits & (1 << 1))
		{
			DeBuffType.NetSerialize(Ar, Map, bOutSuccessLocal);
			bOutSuccess &= bOutSuccessLocal;
		}
		if (RepBits & (1 << 2))
		{
			Ar << bIsSuccessfulDeBuff;
			Ar << DeBuffDamage;
			Ar << DeBuffDuration;
			Ar << DeBuffFrequency;
		}
		return bOutSuccessLocal;
	}
};

