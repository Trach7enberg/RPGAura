// Copyright GGBAO 


#include "GAS/AttributeSet/BaseAttributeSet.h"

#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/Globals/GameAbilitySystemGlobals.h"
#include "Interfaces/CombatInterface.h"
#include "Interfaces/PlayerInterface.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseAttributeSetLog, All, All);

UBaseAttributeSet::UBaseAttributeSet()
{
	CurrentMyAbilitySystemComponent = nullptr;

	const auto GameplayTags = FRPGAuraGameplayTags::Get();
	// FAttributeSignature Signature;
	// Signature.BindStatic(GetStrengthAttribute);
	// TagToAttributeMap.Add(GameplayTags.Attribute_Primary_Strength, Signature);

	TagToAttributeMap.Add(GameplayTags.Attribute_Vital_CurrentHealth, GetCurrentHealthAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Vital_CurrentMana, GetCurrentManaAttribute);

	TagToAttributeMap.Add(GameplayTags.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Primary_Resilience, GetResilienceAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Primary_Strength, GetStrengthAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Primary_Vigor, GetVigorAttribute);

	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_Armor, GetArmorAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);

	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagToAttributeMap.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);

	TagToAttributeMap.Add(GameplayTags.Attributes_Secondary_Resistance_Fire, GetFireResistanceAttribute);
	TagToAttributeMap.Add(GameplayTags.Attributes_Secondary_Resistance_Physical, GetPhysicalResistanceAttribute);
	TagToAttributeMap.Add(GameplayTags.Attributes_Secondary_Resistance_Lightning, GetLightingResistanceAttribute);
	TagToAttributeMap.Add(GameplayTags.Attributes_Secondary_Resistance_Arcane, GetArcaneResistanceAttribute);
}

void UBaseAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldValue) const
{
	// 通知GAS currentHealth刚刚被复制了,使GAS整个系统协调工作,让能力系统记录这一变化并保存旧值的跟踪,以防止需要回滚
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CurrentHealth, OldValue);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldValue);
}

void UBaseAttributeSet::OnRep_CurrentMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CurrentMana, OldValue);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldValue);
}

void UBaseAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Strength, OldValue);
}

void UBaseAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Intelligence, OldValue);
}

void UBaseAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Resilience, OldValue);
}

void UBaseAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Vigor, OldValue);
}

void UBaseAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, Armor, OldValue);
}

void UBaseAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ArmorPenetration, OldValue);
}

void UBaseAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, BlockChance, OldValue);
}

void UBaseAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitChance, OldValue);
}

void UBaseAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitResistance, OldValue);
}

void UBaseAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CriticalHitDamage, OldValue);
}

void UBaseAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, HealthRegeneration, OldValue);
}

void UBaseAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ManaRegeneration, OldValue);
}

void UBaseAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, FireResistance, OldValue);
}

void UBaseAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, PhysicalResistance, OldValue);
}

void UBaseAttributeSet::OnRep_LightingResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, LightingResistance, OldValue);
}

void UBaseAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, ArcaneResistance, OldValue);
}

void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Vital
	// 注册health变量用于复制,这是复制任何变量所需的步骤
	// 复制条件 COND_None ,也就是没有条件的复制
	// REPNOTIFY_Always 意味着如果在服务器上设置了值,就复制给客户端,那个值将被更新和设置,这里的值就是我们的CurrentHealth
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CurrentHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CurrentMana, COND_None, REPNOTIFY_Always);

	// 主要属性
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	// 次要属性
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, LightingResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UBaseAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	if (Attribute == GetCurrentHealthAttribute()) { NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth()); }
	else if (Attribute == GetCurrentManaAttribute()) { NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana()); }
}

void UBaseAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (bIsLevelingUpHealth && Attribute == GetMaxHealthAttribute())
	{
		SetCurrentHealth(NewValue);
		bIsLevelingUpHealth = false;
	}else if(bIsLevelingUpMana && Attribute == GetMaxManaAttribute())
	{
		SetCurrentMana(NewValue);
		bIsLevelingUpMana = false;
	}
}




void UBaseAttributeSet::UpdateCurrentGeProp(const FGameplayEffectModCallbackData& Data, FEffectProp& EffectProp)
{
	const auto Context = Data.EffectSpec.GetContext().Get();
	if (!Context) { return; }

	// Context里的Source是 发起GE的Actor , Target则是被GE影响的Actor
	const auto TargetAsc = Context->GetOriginalInstigatorAbilitySystemComponent();
	EffectProp.EffectContextHandle = Data.EffectSpec.GetEffectContext();
	if (TargetAsc && TargetAsc->AbilityActorInfo.IsValid() && TargetAsc->AbilityActorInfo.Get()->AvatarActor.IsValid())
	{
		EffectProp.TargetAsc = TargetAsc;
		EffectProp.TargetAvatar = TargetAsc->AbilityActorInfo.Get()->AvatarActor.Get();
		EffectProp.TargetController = Cast<AController>(TargetAsc->AbilityActorInfo.Get()->PlayerController);
		EffectProp.TargetCharacter = Cast<ACharacter>(EffectProp.TargetAvatar);
	}

	const auto SourceAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
		Cast<AActor>(Context->GetSourceObject()));

	EffectProp.SourceAsc = SourceAsc;
	if (SourceAsc && SourceAsc->AbilityActorInfo.Get()->PlayerController.IsValid())
	{
		EffectProp.SourceController = Cast<AController>(SourceAsc->AbilityActorInfo.Get()->PlayerController.Get());
		EffectProp.SourceAvatar = EffectProp.SourceController->GetPawn();
		EffectProp.SourceCharacter = Cast<ACharacter>(EffectProp.SourceAvatar);
	}
	else { EffectProp.SourceAvatar = Cast<AActor>(Context->GetSourceObject()); }
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (!GetMyCurrentAbilitySystem())
	{
		UE_LOG(UBaseAttributeSetLog, Error, TEXT("能力系统组件为空!"));
		return;
	}

	const auto Instigator = Data.EffectSpec.GetContext().GetEffectCauser();
	const auto Sufferer = GetMyCurrentAbilitySystem()->GetAvatarActor();
	const auto InstigatorCharacter = Cast<ACharacter>(Instigator);

	// 检查是否等于 InComingDamage 元属性
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute() && GetInComingDamage() != 0)
	{
		const auto TempValue = GetInComingDamage();
		SetInComingDamage(0);


		const auto TempHealth = GetCurrentHealth() - TempValue;

		SetCurrentHealth(FMath::Clamp(TempHealth, 0.f, GetMaxHealth()));

		// 转换成Combat接口获取相应函数
		const auto CbInterface = Cast<ICombatInterface>(Sufferer);
		const auto IsBlockHit = UGameAbilitySystemGlobals::IsBlockedHit(Data.EffectSpec.GetContext());
		const auto IsCriticalHit = UGameAbilitySystemGlobals::IsCriticalHit(Data.EffectSpec.GetContext());

		CbInterface->ShowDamageNumber(TempValue, IsBlockHit, IsCriticalHit);

		// 是否是致命伤
		const bool BIsFatal = TempHealth <= 0.f;

		// 非致命伤,给击中的敌人 激活含有标签HitReact的能力
		if (!BIsFatal)
		{
			// 使用该函数需要确保能力蓝图类中的AbilityTags容器里有Effects_HitReact标签
			GetMyCurrentAbilitySystem()->TryActivateAbilitiesByTag(
				FGameplayTagContainer(FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact));
			// UE_LOG(UBaseAttributeSetLog, Error, TEXT("是否暴击[%d] 是否格挡[%d]"), IsCriticalHit, IsBlockHit);
		}
		else // 是致命伤的话
		{
			if (CbInterface)
			{
				CbInterface->Die();
				// 发送经验游戏事件
				SendXpGamePlayEvent(Sufferer, Instigator);
			}
		}
	}
	if (Data.EvaluatedData.Attribute == GetBlockChanceAttribute())
	{
		// UE_LOG(UBaseAttributeSetLog, Error, TEXT("[%s , %s] Current:%.1f , Final:%.1f"), *GetOwningActor()->GetName(),
		//        *Data.EvaluatedData.Attribute.GetName(), GetBlockChance(), Data.EvaluatedData.Attribute.GetNumericValue(this));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		UE_LOG(UBaseAttributeSetLog, Error, TEXT("[%s]IncomigXP , %.1f"), *GetOwningActor()->GetName(),
		       Data.EvaluatedData.Attribute.GetNumericValue(this));

		if (!GetGiSubSystem()) { return; }
		const auto PlayerInterface = Cast<IPlayerInterface>(Instigator);
		const auto PlayerCombInterface = Cast<ICombatInterface>(Instigator);
		if (!PlayerInterface || !PlayerCombInterface) { return; }

		const auto OldLevel = PlayerCombInterface->GetCharacterLevel();
		const auto TempXP = GetIncomingXP();
		SetIncomingXP(0);

		// 给玩家添加经验,会触发相应的回调函数
		PlayerInterface->AddToPlayerXP(TempXP);
		if (PlayerInterface->CanBeLevelUp())
		{
			bIsLevelingUpHealth = true;
			bIsLevelingUpMana = true;
			PlayerInterface->LevelUp();
			SetCurrentHealth(GetMaxHealth());
			SetCurrentMana(GetMaxMana());
			UE_LOG(UBaseAttributeSetLog, Error, TEXT("LevelUp! [%d] >> [%d]"), OldLevel,
			       PlayerCombInterface->GetCharacterLevel());
		}
	}
}

UBaseAbilitySystemComponent* UBaseAttributeSet::GetMyCurrentAbilitySystem()
{
	if (!CurrentMyAbilitySystemComponent)
	{
		// 存储我们自己的ASC组件 方便后面使用
		CurrentMyAbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(
			GetOwningAbilitySystemComponent());
	}
	return CurrentMyAbilitySystemComponent;
}

URPGAuraGameInstanceSubsystem* UBaseAttributeSet::GetGiSubSystem()
{
	if (!GiSubSystem)
	{
		if (!GetOwningActor()->GetGameInstance()) { return nullptr; }

		GiSubSystem = Cast<URPGAuraGameInstanceSubsystem>(
			GetOwningActor()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>());
		if (!GiSubSystem) { return nullptr; }
	}
	return GiSubSystem;
}

bool UBaseAttributeSet::SendXpGamePlayEvent(AActor* Sufferer, AActor* Instigator)
{
	const auto SufferCombatInt = Cast<ICombatInterface>(Sufferer);
	if (!SufferCombatInt) { return false; }

	const auto SufferLevel = SufferCombatInt->GetCharacterLevel();
	const auto SufferCharacterClass = SufferCombatInt->GetCharacterClass();

	if (!GetGiSubSystem())
	{
		UE_LOG(UBaseAttributeSetLog, Error, TEXT("[%s]致命错误!"), *GetNameSafe(this));
		return false;
	}
	const auto XpReward = GetGiSubSystem()->GetXpRewardFromClassAndLevel(SufferCharacterClass, SufferLevel);

	FGameplayEventData PayLoad;
	PayLoad.EventTag = FRPGAuraGameplayTags::Get().Attributes_Meta_InComingXP;
	PayLoad.EventMagnitude = XpReward;

	// 发送GameplayEvent给造成伤害的Actor,这样玩家就能接受到Xp(玩家身上有一个被动能力一直运行着监听事件)
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		Instigator, FRPGAuraGameplayTags::Get().Attributes_Meta_InComingXP, PayLoad);
	return true;
}
