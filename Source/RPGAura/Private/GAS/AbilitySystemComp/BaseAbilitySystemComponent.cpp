// Copyright GGBAO 


#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "Interfaces/PlayerInterface.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseAbilitySystemComponentLog, All, All);

UBaseAbilitySystemComponent::UBaseAbilitySystemComponent() {}

void UBaseAbilitySystemComponent::BeginPlay() { Super::BeginPlay(); }

void UBaseAbilitySystemComponent::InitSetting()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilitySystemComponent::ClientOnGEAppliedToSelf);
}

void UBaseAbilitySystemComponent::AddCharacterDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities,
                                                               const float CharacterLevel, const bool ActiveWhenGive)
{
	if (!Abilities.Num()) { return; }
	for (const auto& AbilityClass : Abilities)
	{
		AddCharacterDefaultAbility(AbilityClass, CharacterLevel, ActiveWhenGive);
	}
}

void UBaseAbilitySystemComponent::AddCharacterDefaultAbility(const TSubclassOf<UGameplayAbility>& AbilityClass,
                                                             const float CharacterLevel, bool ActiveWhenGive)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CharacterLevel);

	if (ActiveWhenGive) { GiveAbilityAndActivateOnce(AbilitySpec); }
	else
	{
		const auto MyAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability);
		if (!MyAbility)
		{
			UE_LOG(UBaseAbilitySystemComponentLog, Warning, TEXT("%s 转换 失败"), *AbilitySpec.Ability.GetName())
			return;
		}
		// 给能力添加标签
		AbilitySpec.DynamicAbilityTags.AddTag(MyAbility->DefaultInputTag);
		GiveAbility(AbilitySpec);
	}
}


void UBaseAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) { return; }

	// 获取可以激活的能力数组
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		// DynamicAbilityTags 中保存的是与该能力动态相关的标签，而不是全部标签,表示在能力激活或执行过程中可能变化的标签
		// 如果能力的动态标签包含InputTag,那么就可以被激活
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 通知能力系统,输入已经按下,该函数内部会调用虚函数InputPressed,如果你想在这里干别的事情,可以通过覆写虚函数InputPressed实现
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				// 尝试激活能力而不是直接调用激活能力函数,是因为有时候能力可能有一些tag,不允许被激活
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) { return; }

	// 获取可以激活的能力数组
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		// DynamicAbilityTags 中保存的是与该能力动态相关的标签，而不是全部标签,表示在能力激活或执行过程中可能变化的标签
		// 如果能力的动态标签包含InputTag,那么就可以被激活
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 通知能力系统,输入已经按下,该函数内部会调用虚函数InputPressed,如果你想干别的事情,可以在虚函数InputPressed中实现
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				// 尝试激活能力,因为有时候能力可能有一些tag,不允许被激活
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) { return; }

	// 获取可以激活的能力数组
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (AbilitySpec.IsActive())
			{
				// 通知能力系统,输入已经释放 , 并没有终止能力!
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
	}
}

void UBaseAbilitySystemComponent::TryActivateAbilityByDefaultInputTag(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) { return; }

	// 获取可以激活的能力数组
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag)) { TryActivateAbility(AbilitySpec.Handle); }
	}
}

FGameplayTag UBaseAbilitySystemComponent::GetTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec,
                                                                const FGameplayTag& TargetTag)
{
	if (!AbilitySpec.Ability) { return FGameplayTag(); }

	for (auto& Tag : AbilitySpec.Ability.Get()->AbilityTags) { if (Tag.MatchesTag(TargetTag)) { return Tag; } }

	return FGameplayTag();
}

FGameplayTag UBaseAbilitySystemComponent::GetTagFromAbilitySpecDynamicTags(const FGameplayAbilitySpec& AbilitySpec,
                                                                           const FGameplayTag& TargetTag)
{
	if (!AbilitySpec.Ability) { return FGameplayTag(); }


	for (auto& Tag : AbilitySpec.DynamicAbilityTags) { if (Tag.MatchesTag(TargetTag)) { return Tag; } }

	return FGameplayTag();
}

void UBaseAbilitySystemComponent::BroadCastDefaultActivatableAbilitiesInfo()
{
	const auto Gi = GetOwner()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }

	// 安全地遍历所有可激活的能力，当前范围将被锁定,如果有新增加或者被移除的能力不会改变GetActivatableAbilities的结果,完成当前范围之后才会改变
	FScopedAbilityListLock AbilityListLock(*this);

	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		FTagToAbilityInfo AbilityInfo = Gi->AbilityInfoAsset->FindAbilityInfoByAbilityTag(
			GetTagFromAbilitySpec(AbilitySpec, FRPGAuraGameplayTags::Get().Abilities_Attack));

		// 从能力里获得触发该能力对应的输入键(标签)
		AbilityInfo.InputTag = GetTagFromAbilitySpecDynamicTags(AbilitySpec, FRPGAuraGameplayTags::Get().InputTag);
		if (AbilityInfo.InfoDataIsValid()) { Gi->AbilityInfoDelegate.Broadcast(AbilityInfo); }
	}
}

void UBaseAbilitySystemComponent::UpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	if (!GetAvatarActor()) { return; }
	const auto PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor());
	if (!PlayerInterface || PlayerInterface->GetCurrentAssignableAttributePoints() <= 0) { return; }

	FGameplayEventData PayLoad;
	PayLoad.EventTag = AttributeTag;
	PayLoad.EventMagnitude = 1.f;

	// 发送游戏事件给我们的被动能力(有一个EventBase的被动能力会等待Attributes开头的属性标签事件)
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),AttributeTag,PayLoad);

	PlayerInterface->AddToAttributesPoints(-1);
	
	
}

void UBaseAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	// TODO 只广播一次,而不是每次OnRep都广播
	BroadCastDefaultActivatableAbilitiesInfo();
}

void UBaseAbilitySystemComponent::ClientOnGEAppliedToSelf_Implementation(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& GameplayEffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// UE_LOG(UBaseAbilitySystemComponentLog, Warning, TEXT("[%s]In , GeSpec[%s]"),*GetOwner()->GetName(),*GameplayEffectSpec.Def.GetName());


	FGameplayTagContainer Tags;


	GameplayEffectSpec.GetAllAssetTags(Tags);

	if (Tags.Num()) { OnGetAssetTagsDelegate.Broadcast(Tags); }
}
