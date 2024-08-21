// Copyright GGBAO 


#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"

#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseAbilitySystemComponentLog, All, All);

UBaseAbilitySystemComponent::UBaseAbilitySystemComponent() {}

void UBaseAbilitySystemComponent::BeginPlay() { Super::BeginPlay(); }

void UBaseAbilitySystemComponent::InitSetting()
{
	
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilitySystemComponent::ClientOnGEAppliedToSelf);
}

void UBaseAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities)
{
	if (!Abilities.Num()) { return; }
	for (const auto& AbilityClass : Abilities)
	{
		AddCharacterAbility(AbilityClass);
	}
}

void UBaseAbilitySystemComponent::AddCharacterAbility(const TSubclassOf<UGameplayAbility>& AbilityClass)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass);
	const auto MyAbility = Cast<UBaseGameplayAbility>(AbilitySpec.Ability);

	if (!MyAbility)
	{
		UE_LOG(UBaseAbilitySystemComponentLog, Warning, TEXT("%s 转换 失败"), *AbilitySpec.Ability.GetName())
		return;
	}

	// 给能力添加标签
	AbilitySpec.DynamicAbilityTags.AddTag(MyAbility->StartUpTag);
	// GiveAbilityAndActivateOnce(AbilitySpec);
	GiveAbility(AbilitySpec);
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

void UBaseAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) { return; }

	// 获取可以激活的能力数组
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag)) { TryActivateAbility(AbilitySpec.Handle); }
	}
}

void UBaseAbilitySystemComponent::ClientOnGEAppliedToSelf_Implementation(
	UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& GameplayEffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	
	// UE_LOG(UBaseAbilitySystemComponentLog, Warning, TEXT("[%s]In , GeSpec[%s]"),*GetOwner()->GetName(),*GameplayEffectSpec.Def.GetName());
	
	
	FGameplayTagContainer Tags;

	
	GameplayEffectSpec.GetAllAssetTags(Tags);

	if (Tags.Num())
	{
		
		OnGetAssetTagsDelegate.Broadcast(Tags);
	}
}

