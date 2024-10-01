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

FGameplayTag UBaseAbilitySystemComponent::GetAbilityStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const auto& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FRPGAuraGameplayTags::Get().Abilities_Status)) { return Tag; }
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* UBaseAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid()) { return nullptr; }

	FScopedAbilityListLock AbilityListLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		// 能力标签被添加在该能力的AbilityTags容器中,不是dynamicTags中
		if (AbilitySpec.Ability.Get()->AbilityTags.HasTag(AbilityTag)) { return &AbilitySpec; }
	}
	return nullptr;
}

void UBaseAbilitySystemComponent::UpdateAbilityStatus(const FGameplayTag& AbilityTag,
                                                      const FGameplayTag& AbilityStatusTag, const int32 AbilityLevel)
{
	if (!AbilityTag.IsValid() || !AbilityStatusTag.IsValid()) { return; }

	ClientOnAbilityStatusChanged(AbilityTag, AbilityStatusTag, AbilityLevel);
}

void UBaseAbilitySystemComponent::UpdateAbilityStatusWhenLevelUp(const int32 Level)
{
	if (Level < 1) { return; }

	const auto AbilityInfo = URPGAuraGameInstanceSubsystem::GetAbilityInfoAsset(GetAvatarActor());
	if (!AbilityInfo) { return; }

	// 遍历能力信息数据资产,
	for (const auto& Info : AbilityInfo->AbilityInfosOffensive)
	{
		if (Level < Info.LevelRequirement || !Info.AbilityTag.IsValid()) { continue; }

		// 通过给定的能力标签在已激活的能力数组中查找含有该标签的能力
		const auto Spec = GetSpecFromAbilityTag(Info.AbilityTag);

		// 获取到能力说明能力已存在则跳过,我们需要的是未获得的能力
		if (Spec) { continue; }

		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);

		// 把能力的状态设置为Eligible,并给予能力但是不激活 
		AddAbilityStatusTagToTagContainer(GameplayAbilitySpec.DynamicAbilityTags,
		                                  FRPGAuraGameplayTags::Get().Abilities_Status_Eligible);
		GiveAbility(GameplayAbilitySpec);

		// 标记刚刚给予的能力为脏值,强迫该AbilitySpec立马进行Replicate,而不是等到下一个更新
		// 例如我们的法术菜单上的技能球的状态显示要即使的,所以我们得立即进行复制
		MarkAbilitySpecDirty(GameplayAbilitySpec);
		UpdateAbilityStatus(Info.AbilityTag, FRPGAuraGameplayTags::Get().Abilities_Status_Eligible,
		                    GameplayAbilitySpec.Level);
	}
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
                                                             const float CharacterLevel, const bool ActiveWhenGive)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CharacterLevel);

	AddAbilityStatusTagToTagContainer(AbilitySpec.DynamicAbilityTags,
	                                  FRPGAuraGameplayTags::Get().Abilities_Status_Equipped);

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
	if (!AbilitySpec.Ability.Get()) { return FGameplayTag(); }

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
	if (!GetOwner()) { return; }

	const auto Gi = GetOwner()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }

	const auto Infos = URPGAuraGameInstanceSubsystem::GetAbilityInfoAsset(GetOwner());
	if (Infos == nullptr)
	{
		UE_LOG(UBaseAbilitySystemComponentLog, Error, TEXT("[%s]获取技能信息消息数据资产表失败"), *GetNameSafe(this));
		return;
	}

	// 安全地遍历所有可激活的能力，当前范围将被锁定,如果有新增加或者被移除的能力不会改变GetActivatableAbilities的结果,完成当前范围之后才会改变
	FScopedAbilityListLock AbilityListLock(*this);

	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		const auto TempAbilityTag = GetTagFromAbilitySpec(AbilitySpec, FRPGAuraGameplayTags::Get().Abilities_Attack);

		// 能力标签找不到就不需要进行下面步骤了
		if (!TempAbilityTag.IsValid()) { continue; }

		FTagToAbilityInfo AbilityInfo = Infos->FindOffensiveAbilityInfo(TempAbilityTag);

		// 从能力里获得触发该能力对应的输入键(标签)
		AbilityInfo.InputTag = GetTagFromAbilitySpecDynamicTags(AbilitySpec, FRPGAuraGameplayTags::Get().InputTag);
		AbilityInfo.StatusTag = GetAbilityStatusFromSpec(AbilitySpec);
		if (AbilityInfo.InfoDataInputIsValid()) { Gi->AbilityInfoDelegate.Broadcast(AbilityInfo); }
	}
}

void UBaseAbilitySystemComponent::BroadCastDefaultSpellButtonAbilitiesInfo()
{
	if (!GetOwner()) { return; }

	const auto Gi = GetOwner()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	if (!Gi) { return; }

	const auto Infos = URPGAuraGameInstanceSubsystem::GetAbilityInfoAsset(GetOwner());
	if (Infos == nullptr)
	{
		UE_LOG(UBaseAbilitySystemComponentLog, Error, TEXT("[%s]获取技能信息消息数据资产表失败"), *GetNameSafe(this));
		return;
	}

	FScopedAbilityListLock AbilityListLock(*this);

	// 遍历数据资产,并且对应的能力广播信息
	for (auto Info : Infos->GetAllAbilityInfos())
	{
		if (!Info.AbilityTag.IsValid()) { continue; }
		const auto Spec = GetSpecFromAbilityTag(Info.AbilityTag);
		if (!Spec) { continue; }

		Info.StatusTag = GetAbilityStatusFromSpec(*Spec);
		if (!Info.InfoDataAbilityIsValid()) { continue; }

		Gi->OnSpellButtonAbilityInfoChange.Broadcast(Info);
	}
}

void UBaseAbilitySystemComponent::UpgradeSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid() || !GetAvatarActor()) { return; }

	const auto PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor());
	if (!PlayerInterface || PlayerInterface->GetCurrentAssignableSpellPoints() <= 0) { return; }

	// 通过能力标签获取AbilitySpec
	const auto Spec = GetSpecFromAbilityTag(AbilityTag);

	if (!Spec) { return; }

	// 从AbilitySpec获取这个能力的能力状态标签
	const auto StatusTag = GetAbilityStatusFromSpec(*Spec);

	if (!StatusTag.IsValid()) { return; }

	FGameplayTag NewStatusTag{};
	// 如果该能力状态是符合条件的话,则消耗点数然后设置状态为Unlock
	if (StatusTag.MatchesTagExact(FRPGAuraGameplayTags::Get().Abilities_Status_Eligible))
	{
		NewStatusTag = FRPGAuraGameplayTags::Get().Abilities_Status_Unlocked;
		AddAbilityStatusTagToTagContainer(Spec->DynamicAbilityTags,
		                                  NewStatusTag,
		                                  FRPGAuraGameplayTags::Get().Abilities_Status_Eligible);
		PlayerInterface->AddToSpellPoints(-1);
	}
	else if (StatusTag.MatchesTagExact(FRPGAuraGameplayTags::Get().Abilities_Status_Equipped) || StatusTag.
		MatchesTagExact(FRPGAuraGameplayTags::Get().Abilities_Status_Unlocked))
	{
		// 当前能力状态已经装备或者解锁的情况下,只允许升级该能力的等级
		// TODO 立即刷新能力的等级需要重新给予该能力,否则需要等待下次激活能力,新等级才生效
		PlayerInterface->AddToSpellPoints(-1);
		Spec->Level += 1;
	}

	// 因为修改了Spec,因此立即更新Spec,而不是在下一个更新
	MarkAbilitySpecDirty(*Spec);
	UpdateAbilityStatus(AbilityTag, NewStatusTag, Spec->Level);
	UE_LOG(UBaseAbilitySystemComponentLog,Error,TEXT("升级成功!"));
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
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, PayLoad);

	PlayerInterface->AddToAttributesPoints(-1);
}

FAbilityDescription UBaseAbilitySystemComponent::GetAbilityDescriptionByAbilityTag(const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid()) { return FAbilityDescription(); }

	// TODO 应该通过数据资产获取对应技能的详细描述
	const auto Spec = GetSpecFromAbilityTag(AbilityTag);
	if (!Spec) { return FAbilityDescription(); }

	const auto MyAbility = Cast<UBaseGameplayAbility>(Spec->Ability.Get());
	if (!MyAbility) { return FAbilityDescription(); }


	return MyAbility->GetAbilityDescription(AbilityTag, Spec->Level);
}

void UBaseAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	// TODO 只广播一次,而不是每次OnRep都广播
	BroadCastDefaultActivatableAbilitiesInfo();
}

void UBaseAbilitySystemComponent::ClientOnAbilityStatusChanged_Implementation(const FGameplayTag& AbilityTag,
                                                                              const FGameplayTag& AbilityStatusTag,
                                                                              int32 AbilityLevel)
{
	OnAbilityStatusChanged.Broadcast(AbilityTag, AbilityStatusTag, AbilityLevel);
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

void UBaseAbilitySystemComponent::AddAbilityStatusTagToTagContainer(FGameplayTagContainer& TagContainer,
                                                                    const FGameplayTag AbilityStatusTag,
                                                                    const FGameplayTag& RemovedTag)
{
	if (TagContainer.HasTagExact(AbilityStatusTag)) { return; }

	if (RemovedTag.IsValid()) { TagContainer.RemoveTag(RemovedTag); }

	TagContainer.AddTag(AbilityStatusTag);
}
