// Copyright GGBAO 


#include "GAS/AttributeSet/BaseAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GameFramework/Character.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "Interfaces/CombatInterface.h"
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
}

void UBaseAttributeSet::OnRep_CurrentHealth(const FGameplayAttributeData& OldHealth) const
{
	// 通知GAS currentHealth刚刚被复制了,使GAS整个系统协调工作,让能力系统记录这一变化并保存旧值的跟踪,以防止需要回滚
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CurrentHealth, OldHealth);
}

void UBaseAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxHealth, OldMaxHealth);
}

void UBaseAttributeSet::OnRep_CurrentMana(const FGameplayAttributeData& OldCurrentMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, CurrentMana, OldCurrentMana);
}

void UBaseAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBaseAttributeSet, MaxMana, OldMaxMana);
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


void UBaseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

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
}

void UBaseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// if (Attribute == GetBlockChanceAttribute())
	// {
	// 	UE_LOG(UBaseAttributeSetLog, Error, TEXT("[%s , %s] , PreOld:%.1f,PreNew:%.1f"), *GetOwningActor()->GetName(),
	// 		   *Attribute.GetName(), Attribute.GetNumericValue(this), NewValue);
	// }
}

void UBaseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	if (Attribute == GetCurrentHealthAttribute())
	{
		const auto temp = NewValue;
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
		// 待修复Bug,血量满了之后 ,获取显示的值没溢出,但实际的值是溢出了的,导致扣血没有反应 (暂时修复) 
		// TODO  (血量、蓝量已修复Clamp失败的问题还是会有小bug,当药水溢出的持续时间没有结束又扣血会造成血量短暂的反复弹跳)
		// UE_LOG(UBaseAttributeSetLog, Error, TEXT("Old:%.1f , Current :%.1f , BaseValue :%.1f , CurrentValue:%.1f"),
		// 	   temp, GetCurrentHealth(), CurrentHealth.GetBaseValue(), CurrentHealth.GetCurrentValue());
	}
	else if (Attribute == GetCurrentManaAttribute())
	{
		const auto temp = NewValue;
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());

		// UE_LOG(UBaseAttributeSetLog, Error, TEXT("Old:%.1f , Current :%.1f"), temp, GetCurrentMana());
	}
	// else if (Attribute == GetMaxHealthAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, DefaultMaxHealth, NewValue);
	// }
	// else if (Attribute == GetMaxManaAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue, DefaultMaxMana, NewValue);
	// }

	// UE_LOG(UBaseAttributeSetLog, Error, TEXT("[%s,%s]:%.1f"),
	//        (GetOwningActor())?*GetOwningActor()->GetName():*FString(""),
	//        *Attribute.GetName(), NewValue);
	// if (Attribute == GetBlockChanceAttribute())
	// {
	// 	UE_LOG(UBaseAttributeSetLog, Error, TEXT("[%s , %s] , Old:%.1f,New:%.1f"), *GetOwningActor()->GetName(),
	// 	       *Attribute.GetName(), Attribute.GetNumericValue(this), NewValue);
	// }
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
		UE_LOG(UBaseAttributeSetLog, Error, TEXT("能力系统组件 为空!"));
		return;
	}

	// 检查是否等于 InComingDamage 元属性
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute() && GetInComingDamage() != 0)
	{
		const auto TempValue = GetInComingDamage();
		SetInComingDamage(0);


		const auto TempHealth = GetCurrentHealth() - TempValue;

		SetCurrentHealth(FMath::Clamp(TempHealth, 0.f, GetMaxHealth()));

		// 转换成Combat接口获取相应函数
		const auto CbInterface = Cast<ICombatInterface>(GetMyCurrentAbilitySystem()->GetAvatarActor());

		const auto Instigator = Data.EffectSpec.GetContext().GetEffectCauser();
		const auto Sufferer = GetMyCurrentAbilitySystem()->GetAvatarActor();
		if (Instigator != Sufferer) { CbInterface->ShowDamageNumber(TempValue); }

		// 是否是致命伤
		const bool BIsFatal = TempHealth <= 0.f;

		// 非致命伤,给击中的敌人 激活含有标签HitReact的能力
		if (!BIsFatal)
		{
			// GetMyCurrentAbilitySystem()->TryActivateAbilityByTag(FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact);

			// 使用该函数需要确保能力蓝图类中的AbilityTags容器里有Effects_HitReact标签
			GetMyCurrentAbilitySystem()->TryActivateAbilitiesByTag(
				FGameplayTagContainer(FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact));
		}
		else { if (CbInterface) { CbInterface->Die(); } }
	}
	if (Data.EvaluatedData.Attribute == GetBlockChanceAttribute() )
	{
		
		// UE_LOG(UBaseAttributeSetLog, Error, TEXT("[%s , %s] Current:%.1f , Final:%.1f"), *GetOwningActor()->GetName(),
		//        *Data.EvaluatedData.Attribute.GetName(), GetBlockChance(), Data.EvaluatedData.Attribute.GetNumericValue(this));
	}else
	{
		// UE_LOG(UBaseAttributeSetLog, Error, TEXT("All-[%s , %s] Final:%.1f"), *GetOwningActor()->GetName(),
		// 	   *Data.EvaluatedData.Attribute.GetName(), Data.EvaluatedData.Attribute.GetNumericValue(this));
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
