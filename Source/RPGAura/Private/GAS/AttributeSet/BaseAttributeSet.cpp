// Copyright GGBAO 


#include "GAS/AttributeSet/BaseAttributeSet.h"

#include "SubSystems/RPGAuraGameInstanceSubsystem.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "CoreTypes/RPGAuraGasCoreTypes.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
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
	}
	else if (bIsLevelingUpMana && Attribute == GetMaxManaAttribute())
	{
		SetCurrentMana(NewValue);
		bIsLevelingUpMana = false;
	}
}

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	UpdateCurrentGeProp(Data, EffectProperties);
	if (!EffectProperties.IsAvatarValid()) { return; }

	// 检查是否等于 InComingDamage 元属性
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute() && GetInComingDamage() != 0)
	{
		const auto TempInComingDamage = GetInComingDamage();
		SetInComingDamage(0);

		const auto TempHealth = GetCurrentHealth() - TempInComingDamage;

		SetCurrentHealth(FMath::Clamp(TempHealth, 0.f, GetMaxHealth()));

		if (const auto CbInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatar); CbInterface && CbInterface->
			IsCharacterDie()) { return; }

		HandleHealthReact(TempInComingDamage, TempHealth);
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
		const auto PlayerInterface = Cast<IPlayerInterface>(EffectProperties.SourceAvatar);
		const auto PlayerCombInterface = Cast<ICombatInterface>(EffectProperties.SourceAvatar);
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

void UBaseAttributeSet::UpdateCurrentGeProp(const FGameplayEffectModCallbackData& Data, FEffectProp& EffectProp)
{
	// TODO 成员内容一样的时候就不需要每次重新更新赋值了 
	FGameplayEffectContextHandle GeContextHandle = Data.EffectSpec.GetContext();
	FGameplayEffectContext* GeContext = GeContextHandle.Get();

	if (!GeContext) { return; }

	const auto MyGeContext = UGameAbilitySystemGlobals::GetCustomGeContext(GeContext);

	EffectProp.EffectContextHandle = GeContextHandle;
	EffectProp.MyGeContext = (MyGeContext) ? *MyGeContext : EffectProp.MyGeContext;

	// Context里的Source是 发起GE的Actor , Target则是被GE影响的Actor(也就是当前属性集的Owner)
	const auto Instigator = Data.EffectSpec.GetContext().GetEffectCauser();
	const auto Sufferer = GetMyCurrentAbilitySystem()->GetAvatarActor();

	EffectProp.SourceAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Instigator);
	EffectProp.SourceAvatar = (EffectProp.SourceAsc) ? EffectProp.SourceAsc->GetAvatarActor() : nullptr;
	EffectProp.SourceCharacter = Cast<ACharacter>(EffectProp.SourceAvatar);
	EffectProp.SourceController = EffectProp.SourceCharacter ? EffectProp.SourceCharacter->GetController() : nullptr;

	EffectProp.TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Sufferer);
	EffectProp.TargetAvatar = (EffectProp.TargetAsc) ? EffectProp.TargetAsc->GetAvatarActor() : nullptr;
	EffectProp.TargetCharacter = Cast<ACharacter>(EffectProp.TargetAvatar);
	EffectProp.TargetController = EffectProp.TargetCharacter ? EffectProp.TargetCharacter->GetController() : nullptr;
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

void UBaseAttributeSet::HandleHealthReact(
	const float TempValue, const float TempHealth)
{
	if (!EffectProperties.IsAvatarValid() || !EffectProperties.IsCharacterValid()) { return; }

	if (!GetMyCurrentAbilitySystem())
	{
		UE_LOG(UBaseAttributeSetLog, Error, TEXT("能力系统组件为空!"));
		return;
	}
	// 转换成Combat接口获取相应函数
	const auto CbInterface = Cast<ICombatInterface>(EffectProperties.TargetAvatar);

	const auto IsBlockHit = UGameAbilitySystemGlobals::IsBlockedHit(EffectProperties.EffectContextHandle);
	const auto IsCriticalHit = UGameAbilitySystemGlobals::IsCriticalHit(EffectProperties.EffectContextHandle);
	CbInterface->ShowDamageNumber(TempValue, IsBlockHit, IsCriticalHit); // TODO 触发DeBuff时候的显示数字会与正常显示重合待修 

	HandleDeBuff();

	// 是否是致命伤
	const bool BIsFatal = TempHealth <= 0.f;

	// const auto TmpDeBuffArray = EffectProperties.MyGeContext.GetDeBuffInfos();
	// if (TmpDeBuffArray.Num() > 0)
	// {
	// 	const auto OffName = GetNameSafe(EffectProperties.SourceAvatar);
	// 	const auto SufferName = GetNameSafe(EffectProperties.TargetAvatar);
	// 	UE_LOG(UBaseAttributeSetLog, Error, TEXT("攻击者[%s] -> 受击[%s],DeBuff数组大小:%d"), *OffName, *SufferName,
	// 	       TmpDeBuffArray.Num());
	// 	UE_LOG(UBaseAttributeSetLog, Error, TEXT("攻击者[%s] -> 受击[%s],Debuff[0]是否触发Debuff?: %d , DeBuff名字: %s"), *OffName,
	// 	       *SufferName, TmpDeBuffArray[0].bIsSuccessfulDeBuff, *TmpDeBuffArray[0].DeBuffType.ToString());
	// }

	// 非致命伤,给击中的敌人 激活含有标签HitReact的能力
	if (!BIsFatal)
	{
		// 使用该函数需要确保能力蓝图类中的AbilityTags容器里有Effects_HitReact标签
		GetMyCurrentAbilitySystem()->TryActivateAbilitiesByTag(
			FGameplayTagContainer(FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact));
		// UE_LOG(UBaseAttributeSetLog, Error, TEXT("是否暴击[%d] 是否格挡[%d]"), IsCriticalHit, IsBlockHit);
		if (!EffectProperties.MyGeContext.IsDeBuffSideEffect() && EffectProperties.MyGeContext.IsKnockBackHit())
		{
			CbInterface->AddKnockBack(EffectProperties.MyGeContext.GetImpulse());
		}
	}
	else // 是致命伤的话
	{
		if (CbInterface)
		{
			CbInterface->Die();
			// 发送经验游戏事件
			SendXpGamePlayEvent(EffectProperties.TargetAvatar, EffectProperties.SourceAvatar);
			CbInterface->AddDeathImpulse(EffectProperties.MyGeContext.GetImpulse());
		}
	}
}


void UBaseAttributeSet::HandleDeBuff()
{
	if (!EffectProperties.IsAscValid() || !EffectProperties.IsAvatarValid() || !EffectProperties.IsGeContextValid())
	{
		return;
	}

	const auto TargetCombInt = Cast<ICombatInterface>(EffectProperties.TargetAvatar);
	if (!TargetCombInt) { return; }

	// 创建动态GE
	for (auto& DeBuffInfo : EffectProperties.MyGeContext.GetDeBuffInfos())
	{
		if (!DeBuffInfo.bIsSuccessfulDeBuff) { continue; }

		// 通过Instigator	的ASC创建新的 FGameplayEffectContextHandle
		auto GeContextHandle = EffectProperties.SourceAsc->MakeEffectContext();
		GeContextHandle.AddSourceObject(EffectProperties.SourceAvatar);

		// 动态GE的名字
		FString DeBuffName = FString::Printf(TEXT("DynamicDeBuff_%s"), *DeBuffInfo.DeBuffType.ToString());

		// 在UE中,包Package是对资源进行组织和管理的方式
		// GetTransientPackage() 返回的临时包对象用于在运行时动态创建对象和资源，而无需将其永久保存到磁盘上的包文件中
		// 当代码需要在运行时创建临时资源（如动态创建GE、蓝图或其他对象）时
		// 可以使用 GetTransientPackage() 函数来获取一个临时的包对象,并将新创建的对象分配给该包
		// 这样做可以方便地管理这些临时资源，而无需创建和维护额外的包文件
		// 即临时包对象在游戏运行期间存在,并且不会被保存到磁盘上的包文件中一旦游戏结束或重新编译项目,这里new的GE将被清理并释放内存
		UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DeBuffName));
		if (!GameplayEffect) { continue; }

		GameplayEffect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
		GameplayEffect->Period = DeBuffInfo.DeBuffFrequency;
		GameplayEffect->DurationMagnitude = FScalableFloat(DeBuffInfo.DeBuffDuration);
		// 按施法者设置堆叠策略
		GameplayEffect->StackingType = EGameplayEffectStackingType::AggregateBySource;
		GameplayEffect->StackLimitCount = 1;

		// 添加GE组件,用于添加授予目标的标签 TODO 待修改const_cast 
		UTargetTagsGameplayEffectComponent& GeComp = GameplayEffect->AddComponent<UTargetTagsGameplayEffectComponent>();
		auto& TagContainer = const_cast<FInheritedTagContainer&>(GeComp.GetConfiguredTargetTagChanges());
		TagContainer.Added.AddTag(DeBuffInfo.DeBuffType);
		GeComp.SetAndApplyTargetTagChanges(TagContainer);

		// 新建Modifier并且配置Modifier信息
		GameplayEffect->Modifiers.Add(FGameplayModifierInfo());
		auto& GameplayModifierInfo = GameplayEffect->Modifiers.Last();
		GameplayModifierInfo.ModifierMagnitude = FScalableFloat(DeBuffInfo.DeBuffDamage);
		GameplayModifierInfo.ModifierOp = EGameplayModOp::Additive;
		GameplayModifierInfo.Attribute = UBaseAttributeSet::GetInComingDamageAttribute();

		const auto Level = EffectProperties.MyGeContext.GetAbilityLevel();
		const auto Str = EffectProperties.MyGeContext.GetImpulse().ToString();
		UE_LOG(UBaseAttributeSetLog, Warning, TEXT("[前向向量: , %s]"), *Str);

		// 通过GE上下文句柄和创建的GE new 一个GE Spec,以便我们Apply
		if (const FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(GameplayEffect, GeContextHandle, Level))
		{
			// 我的蓝图函数库,用于从GE上下文转换为我自己的GE自定义上下文
			if (const auto TmpGeContext =
				UGameAbilitySystemGlobals::GetCustomGeContext(MutableSpec->GetContext().Get()))
			{
				TmpGeContext->AddDamageType(DeBuffInfo.DamageType);
				// 注意DeBuff所产生的负面GE上下文里携带的所有DeBuffInfo信息里的bIsSuccessfulDeBuff一定要为false,或者没有,否则会产生无限循环
				TmpGeContext->SetIsDeBuffSideEffect(true);
				EffectProperties.TargetAsc->ApplyGameplayEffectSpecToSelf(*MutableSpec);
				TargetCombInt->ShowDeBuffVfx(DeBuffInfo.DeBuffType);
			}
		}
	}
}
