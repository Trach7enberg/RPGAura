// Copyright GGBAO 


#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/CharacterClassInfo.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"
#include "GAS/GameplayAbilities/BaseGameplayAbility.h"
#include "Interfaces/PlayerInterface.h"
#include "Save/LoadScreen/LoadScreenSave.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseAbilitySystemComponentLog, All, All);

UBaseAbilitySystemComponent::UBaseAbilitySystemComponent() {}

void UBaseAbilitySystemComponent::BeginPlay() { Super::BeginPlay(); }

void UBaseAbilitySystemComponent::InitSetting()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UBaseAbilitySystemComponent::ClientOnGEAppliedToSelf);
}

URPGAuraGameInstanceSubsystem* UBaseAbilitySystemComponent::GetMyGiSystem()
{
	if (!GameInstanceSubsystem)
	{
		if (!GetOwner()) { return nullptr; }
		GameInstanceSubsystem = GetOwner()->GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>();
	}

	return GameInstanceSubsystem.Get();
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

FGameplayAbilitySpec* UBaseAbilitySystemComponent::GetSpecFromInputTag(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid() || !InputTag.MatchesTag(FRPGAuraGameplayTags::Get().InputTag)) { return nullptr; }

	FScopedAbilityListLock AbilityListLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		// 能力标签被添加在该能力的AbilityTags容器中,不是dynamicTags中
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)) { return &AbilitySpec; }
	}
	return nullptr;
}

FGameplayTag UBaseAbilitySystemComponent::GetSpecAbilityStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	const auto Spec = GetSpecFromAbilityTag(AbilityTag);

	if (!Spec) { return FGameplayTag(); }

	return GetAbilityStatusFromSpec(*Spec);
}

void UBaseAbilitySystemComponent::UpdateAbilityStatus(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& AbilityStatusTag,
	const int32         AbilityLevel)
{
	if (!AbilityTag.IsValid() || !AbilityStatusTag.IsValid()) { return; }

	ClientOnAbilityStatusChanged(AbilityTag, AbilityStatusTag, AbilityLevel);
}

void UBaseAbilitySystemComponent::GetAbilityToSave(TArray<FSavedAbility>& AbilityToSave)
{
	AbilityToSave.Reset();
	if (!GetMyGiSystem()) { return; }
	const auto InfoAsset = GetMyGiSystem()->GetAbilityInfoAsset();
	if (!InfoAsset) { return; }
	FScopedAbilityListLock AbilityListLock(*this);
	// 获取可以激活的能力数组 
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		auto AbilityTag = GetTagFromAbilitySpec(AbilitySpec, FRPGAuraGameplayTags::Get().Abilities_Attack);
		if (!AbilityTag.IsValid())
		{
			// 如果主动攻击技能不存在,查找被动技能
			AbilityTag = GetTagFromAbilitySpec(AbilitySpec, FRPGAuraGameplayTags::Get().Abilities_Passive);
		}
		if (!AbilityTag.IsValid()) { continue; }

		const auto AbilityInfo = InfoAsset->FindAbilityInfo(AbilityTag);
		if (!AbilityInfo.AbilityTag.IsValid() || !AbilityInfo.
			AbilityClass) { continue; }

		bool IsPassiveOrOffensive = false;
		IsPassiveOrOffensive      =
				AbilityInfo.AbilityType == FRPGAuraGameplayTags::Get().Abilities_Type_Offensive ||
				AbilityInfo.AbilityType == FRPGAuraGameplayTags::Get().Abilities_Type_Passive;
		if (!IsPassiveOrOffensive) { continue; }

		FSavedAbility SavedAbility{};
		SavedAbility.AbilityTag       = AbilityTag;
		SavedAbility.AbilityLevel     = AbilitySpec.Level;
		SavedAbility.AbilityType      = AbilityInfo.AbilityType;
		SavedAbility.GameplayAbility  = AbilityInfo.AbilityClass;
		SavedAbility.AbilityStatus    = GetAbilityStatusFromSpec(AbilitySpec);
		SavedAbility.AbilityInputSlot = GetTagFromAbilitySpecDynamicTags(AbilitySpec,
		                                                                 FRPGAuraGameplayTags::Get().InputTag);
		AbilityToSave.Add(SavedAbility);
	}
}

void UBaseAbilitySystemComponent::AddAbilitiesFromLoadData(ULoadScreenSave* LoadScreenSave)
{
	if (!LoadScreenSave || !LoadScreenSave->SavedAbilities.Num() || !GetMyGiSystem()) { return; }

	const auto AbilityInfoAsset = GetMyGiSystem()->GetAbilityInfoAsset();
	if (!AbilityInfoAsset) { return; }

	for (const auto& SavedAbility : LoadScreenSave->SavedAbilities)
	{
		if (!SavedAbility.GameplayAbility) { continue; }
		FGameplayAbilitySpec AbilitySpec =
				FGameplayAbilitySpec(SavedAbility.GameplayAbility, SavedAbility.AbilityLevel);

		AddTagToAbilitySpecContainer(AbilitySpec.DynamicAbilityTags,
		                             SavedAbility.AbilityStatus);
		AddTagToAbilitySpecContainer(AbilitySpec.DynamicAbilityTags,
		                             SavedAbility.AbilityInputSlot);

		// 处理被动技能
		if (SavedAbility.AbilityType == FRPGAuraGameplayTags::Get().Abilities_Type_Passive)
		{
			if (SavedAbility.AbilityStatus == FRPGAuraGameplayTags::Get().Abilities_Status_Equipped)
			{
				GiveAbilityAndActivateOnce(AbilitySpec);
			}
			else { GiveAbility(AbilitySpec); }
		}
		else { GiveAbility(AbilitySpec); }
	}
}

void UBaseAbilitySystemComponent::AddListenPassiveAbilities(
	const ECharacterClass CharacterClass,
	const float           AbilityLevel)
{
	if (!GetMyGiSystem()) { return; }

	const auto GiSubSystem = GetMyGiSystem();

	if (!GiSubSystem->CharacterClassInfo)
	{
		UE_LOG(UBaseAbilitySystemComponentLog, Error, TEXT("[%s]初始能力对象为空!"), *GetName());
		return;
	}
	
	const FCharacterClassDefaultInfo CcdI = GiSubSystem->CharacterClassInfo->FindClassDefaultInfo(CharacterClass);

	if (!CcdI.StartUpPassiveAbilities.Num()) { return; }

	// 赋予并激活角色相应的初始(监听)被动能力
	AddCharacterDefaultAbilities(CcdI.StartUpPassiveAbilities, AbilityLevel, true);
}

void UBaseAbilitySystemComponent::UpdateAbilityStatusWhenLevelUp(const int32 Level)
{
	if (Level < 1) { return; }

	const auto AbilityInfo = (GetMyGiSystem()) ? GetMyGiSystem()->GetAbilityInfoAsset() : nullptr;
	if (!AbilityInfo) { return; }

	// 遍历能力信息数据资产,
	for (const auto& Info : AbilityInfo->GetAllAbilityInfos())
	{
		if (Level < Info.LevelRequirement || !Info.AbilityTag.IsValid()) { continue; }

		// 通过给定的能力标签在已激活的能力数组中查找含有该标签的能力
		const auto Spec = GetSpecFromAbilityTag(Info.AbilityTag);

		// 获取到能力说明能力已存在则跳过,我们需要的是未获得的能力
		if (Spec) { continue; }

		FGameplayAbilitySpec GameplayAbilitySpec = FGameplayAbilitySpec(Info.AbilityClass, 1);

		// 把能力的状态设置为Eligible,并给予能力但是不激活 
		AddTagToAbilitySpecContainer(GameplayAbilitySpec.DynamicAbilityTags,
		                             FRPGAuraGameplayTags::Get().Abilities_Status_Eligible);
		GiveAbility(GameplayAbilitySpec);

		// 标记刚刚给予的能力为脏值,强迫该AbilitySpec立马进行Replicate,而不是等到下一个更新
		// 例如我们的法术菜单上的技能球的状态显示要即使的,所以我们得立即进行复制
		MarkAbilitySpecDirty(GameplayAbilitySpec);
		UpdateAbilityStatus(Info.AbilityTag,
		                    FRPGAuraGameplayTags::Get().Abilities_Status_Eligible,
		                    GameplayAbilitySpec.Level);
	}
}

void UBaseAbilitySystemComponent::AddCharacterDefaultAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& Abilities,
	const float                                  CharacterLevel,
	const bool                                   ActiveWhenGive)
{
	if (!Abilities.Num()) { return; }
	for (const auto& AbilityClass : Abilities)
	{
		AddCharacterDefaultAbility(AbilityClass, CharacterLevel, ActiveWhenGive);
	}
}

void UBaseAbilitySystemComponent::AddCharacterDefaultAbility(
	const TSubclassOf<UGameplayAbility>& AbilityClass,
	const float                          CharacterLevel,
	const bool                           ActiveWhenGive)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CharacterLevel);

	AddTagToAbilitySpecContainer(AbilitySpec.DynamicAbilityTags,
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
		if (MyAbility->DefaultInputTag.IsValid()) { AbilitySpec.DynamicAbilityTags.AddTag(MyAbility->DefaultInputTag); }

		GiveAbility(AbilitySpec);
	}
}


void UBaseAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) { return; }
	UE_LOG(UBaseAbilitySystemComponentLog, Warning, TEXT("[%s]按下"), *InputTag.ToString())


	// 获取可以激活的能力数组
	FScopedAbilityListLock AbilityListLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		// DynamicAbilityTags 中保存的是与该能力动态相关的标签，而不是全部标签,表示在能力激活或执行过程中可能变化的标签
		// 如果能力的动态标签包含InputTag,那么就可以被激活
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 通知能力系统,输入已经按下,该函数内部会调用虚函数InputPressed,如果你想在这里干别的事情,可以通过覆写虚函数InputPressed实现
			AbilitySpecInputPressed(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed,
			                      AbilitySpec.Handle,
			                      AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			if (!AbilitySpec.IsActive()) { TryActivateAbility(AbilitySpec.Handle); }
		}
	}
}

void UBaseAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) { return; }

	FScopedAbilityListLock AbilityListLock(*this);
	// 获取可以激活的能力数组 
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		// DynamicAbilityTags 中保存的是与该能力动态相关的标签，而不是全部标签,表示在能力激活或执行过程中可能变化的标签
		// 如果能力的动态标签包含InputTag,那么就可以被激活
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 通知能力系统,输入已经按下,该函数内部会调用虚函数InputPressed,如果你想干别的事情,可以在虚函数InputPressed中实现
			// AbilitySpecInputPressed(AbilitySpec);
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

	FScopedAbilityListLock AbilityListLock(*this);
	// 获取可以激活的能力数组
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			// 通知能力系统,输入已经释放,并没有终止能力!
			AbilitySpecInputReleased(AbilitySpec);
			// 向服务器发送数据,告诉它我们正在做某事中
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased,
			                      AbilitySpec.Handle,
			                      AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			break;
		}
	}
}

void UBaseAbilitySystemComponent::TryActivateAbilityByDefaultInputTag(const FGameplayTag& Tag)
{
	if (!Tag.IsValid()) { return; }

	FScopedAbilityListLock AbilityListLock(*this);
	// 获取可以激活的能力数组
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag)) { TryActivateAbility(AbilitySpec.Handle); }
	}
}

FGameplayTag UBaseAbilitySystemComponent::GetTagFromAbilitySpec(
	const FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag&         TargetTag)
{
	if (!AbilitySpec.Ability.Get()) { return FGameplayTag(); }

	for (auto& Tag : AbilitySpec.Ability.Get()->AbilityTags) { if (Tag.MatchesTag(TargetTag)) { return Tag; } }

	return FGameplayTag();
}

FGameplayTag UBaseAbilitySystemComponent::GetTagFromAbilitySpecDynamicTags(
	const FGameplayAbilitySpec& AbilitySpec,
	const FGameplayTag&         TargetTag)
{
	if (!AbilitySpec.Ability) { return FGameplayTag(); }


	for (auto& Tag : AbilitySpec.DynamicAbilityTags) { if (Tag.MatchesTag(TargetTag)) { return Tag; } }

	return FGameplayTag();
}

void UBaseAbilitySystemComponent::BroadCastDefaultActivatableAbilitiesInfo()
{
	if (!GetOwner()) { return; }


	if (!GetMyGiSystem()) { return; }

	const auto Infos = (GetMyGiSystem()) ? GetMyGiSystem()->GetAbilityInfoAsset() : nullptr;
	if (Infos == nullptr)
	{
		UE_LOG(UBaseAbilitySystemComponentLog, Error, TEXT("[%s]获取技能信息消息数据资产表失败"), *GetNameSafe(this));
		return;
	}

	// 安全地遍历所有可激活的能力，当前范围将被锁定,如果有新增加或者被移除的能力不会改变GetActivatableAbilities的结果,完成当前范围之后才会改变
	FScopedAbilityListLock AbilityListLock(*this);
	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		// TODO 换种方式来判断被动和主动技能 
		auto TempAbilityTag = GetTagFromAbilitySpec(AbilitySpec, FRPGAuraGameplayTags::Get().Abilities_Attack);
		if (!TempAbilityTag.IsValid())
		{
			// 看是否为被动能力
			TempAbilityTag = GetTagFromAbilitySpec(AbilitySpec, FRPGAuraGameplayTags::Get().Abilities_Passive);
		}

		// 能力标签找不到就不需要进行下面步骤了
		if (!TempAbilityTag.IsValid()) { continue; }

		FTagToAbilityInfo AbilityInfo = Infos->FindAbilityInfo(TempAbilityTag);

		// TODO 如果初始配置的能力的输入标签有重复会导致显示BUG,待修复   
		// 从能力里获得触发该能力对应的输入键(标签)
		AbilityInfo.InputTag  = GetTagFromAbilitySpecDynamicTags(AbilitySpec, FRPGAuraGameplayTags::Get().InputTag);
		AbilityInfo.StatusTag = GetAbilityStatusFromSpec(AbilitySpec);
		GetMyGiSystem()->AbilityInfoDelegate.Broadcast(AbilityInfo);
	}
}

void UBaseAbilitySystemComponent::BroadCastDefaultSpellButtonAbilitiesInfo()
{
	if (!GetOwner()) { return; }

	if (!GetMyGiSystem()) { return; }

	const auto Infos = (GetMyGiSystem()) ? GetMyGiSystem()->GetAbilityInfoAsset() : nullptr;
	if (Infos == nullptr) { return; }

	FScopedAbilityListLock AbilityListLock(*this);

	// 遍历数据资产,并且对应的能力广播信息
	for (auto Info : Infos->GetAllAbilityInfos())
	{
		if (!Info.AbilityTag.IsValid()) { continue; }
		// 从已激活的能力中查找
		const auto Spec = GetSpecFromAbilityTag(Info.AbilityTag);
		if (!Spec) { continue; }

		Info.StatusTag = GetAbilityStatusFromSpec(*Spec);
		if (!Info.InfoDataAbilityIsValid()) { continue; }

		ClientOnSpellButtonAbilityInfoChange(Info);
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
		AddTagToAbilitySpecContainer(Spec->DynamicAbilityTags,
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
	UE_LOG(UBaseAbilitySystemComponentLog, Error, TEXT("升级成功!"));
}

void UBaseAbilitySystemComponent::UpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	if (!GetAvatarActor()) { return; }
	const auto PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor());
	if (!PlayerInterface || PlayerInterface->GetCurrentAssignableAttributePoints() <= 0) { return; }

	FGameplayEventData PayLoad;
	PayLoad.EventTag       = AttributeTag;
	PayLoad.EventMagnitude = 1.f;

	// 发送游戏事件给我们的被动能力(有一个EventBase的被动能力会等待Attributes开头的属性标签事件)
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, PayLoad);

	PlayerInterface->AddToAttributesPoints(-1);
}

void UBaseAbilitySystemComponent::ServerEquipAbility_Implementation(
	const FGameplayTag& ToEquipAbilityTag,
	const FGameplayTag& NewInputSlot)
{
	if (!GetOwner() || !ToEquipAbilityTag.IsValid() || !NewInputSlot.IsValid()) { return; }

	// 尝试获得准备要装备技能能力的Spec
	const auto ToEquipAbilitySpec = GetSpecFromAbilityTag(ToEquipAbilityTag);
	if (!ToEquipAbilitySpec) { return; }

	// 获取要装备到新插槽的技能的能力的状态标签
	const auto ToEquipAbilityStatus = GetAbilityStatusFromSpec(*ToEquipAbilitySpec);

	// 只有当能力状态是解锁了或则已经装备了才允许更换插槽
	if (!ToEquipAbilityStatus.IsValid()) { return; }

	// 尝试获取NewInputSlot(新插槽上)已经装备的(如果插槽装备了技能的话)能力的OldSpec
	const auto   OldSpec      = GetSpecFromInputTag(NewInputSlot);
	FGameplayTag OldInputSlot = FGameplayTag();

	auto NewAbilityInfo = GetMyGiSystem()->GetAbilityInfoAsset()->
	                                       FindAbilityInfo(ToEquipAbilityTag);
	NewAbilityInfo.InputTag  = NewInputSlot;
	NewAbilityInfo.StatusTag = FRPGAuraGameplayTags::Get().Abilities_Status_Equipped;

	// 从要装备的能力获取当前技能准备装备到法术菜单的区域类型(被动区域还是主动区域)
	const auto CurrentEquipAbilityAreaType = NewAbilityInfo.AbilityType == (FRPGAuraGameplayTags::Get().
		                                         Abilities_Type_Passive)
		                                         ? FRPGAuraGameplayTags::Get().Abilities_Passive
		                                         : FRPGAuraGameplayTags::Get().Abilities_Attack;
	if (!CurrentEquipAbilityAreaType.IsValid()) { return; }

	auto OldAbilityInfo = FTagToAbilityInfo{};

	bool bIsSwapAbilitySlotPos = false;
	// TODO 待判断要装备的技能是否符合对应的插槽(如主动技能只能装在主动技能槽,不能装到被动技能槽
	// TODO 尚未实现从装备菜单中移除技能球功能...   

	// 如果该能力状态是[UnLocked]的,意味着没有装备
	if (ToEquipAbilityStatus.MatchesTagExact(FRPGAuraGameplayTags::Get().Abilities_Status_Unlocked))
	{
		// 成功获取OldSpec说明这个NewInputSlot插槽位置上有已经装备的能力,则需要把这个能力OldSpec里的能力插槽标签清除,并设置插槽标签为None
		// 然后设置这个OldSpec的能力状态为UnLocked
		if (OldSpec)
		{
			AddTagToAbilitySpecContainer(OldSpec->DynamicAbilityTags,
			                             FGameplayTag(),
			                             FRPGAuraGameplayTags::Get().InputTag);
			AddTagToAbilitySpecContainer(OldSpec->DynamicAbilityTags,
			                             FRPGAuraGameplayTags::Get().Abilities_Status_Unlocked,
			                             FRPGAuraGameplayTags::Get().Abilities_Status);

			OldAbilityInfo = GetMyGiSystem()->GetAbilityInfoAsset()->FindAbilityInfo(
			 GetTagFromAbilitySpec(*OldSpec, CurrentEquipAbilityAreaType));

			// 判断NewInputSlot插槽位置上已经装备的能力是否是被动技能,如果是则需要停用能力
			if (OldAbilityInfo.AbilityType == FRPGAuraGameplayTags::Get().Abilities_Type_Passive)
			{
				OnDeActivePassiveAbility.Broadcast(OldAbilityInfo.AbilityTag);
			}
		}

		// 最后设置ToEquipAbilitySpec的插槽标签为NewInputSlot,且能力状态为Equipped
		AddTagToAbilitySpecContainer(ToEquipAbilitySpec->DynamicAbilityTags,
		                             NewInputSlot,
		                             FRPGAuraGameplayTags::Get().InputTag);
		AddTagToAbilitySpecContainer(ToEquipAbilitySpec->DynamicAbilityTags,
		                             FRPGAuraGameplayTags::Get().Abilities_Status_Equipped,
		                             FRPGAuraGameplayTags::Get().Abilities_Status);
		// 判断要装备的技能是否为被动技能,被动技能装备时机会立即启用能力
		if (NewAbilityInfo.AbilityType == FRPGAuraGameplayTags::Get().Abilities_Type_Passive)
		{
			TryActivateAbility(ToEquipAbilitySpec->Handle);
		}
	}
	// 如果要准备装备的能力是[Equipped]的,意味着已经装备在某个插槽上了,无非就是交换位置(即新插槽已经有装备的技能)和换到没有装备技能的插槽上(被动技能交换位置不需要停用)
	else if (ToEquipAbilityStatus.MatchesTagExact(FRPGAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		// 获取当前要装备到新插槽的技能身上所拥有的插槽标签OldInputSlot,如果等于NewInputSlot 说明要装备的位置一样,则<返回>
		OldInputSlot = GetTagFromAbilitySpecDynamicTags(*ToEquipAbilitySpec,
		                                                FRPGAuraGameplayTags::Get().InputTag);
		if (!OldInputSlot.IsValid() || OldInputSlot == NewInputSlot) { return; }

		// 要装备的位置不一样,互相交换输入插槽,即 OldSpec与ToEquipAbilitySpec的插槽标签,它们的能力状态不需要更改,都是Equipped
		AddTagToAbilitySpecContainer(ToEquipAbilitySpec->DynamicAbilityTags,
		                             NewInputSlot,
		                             FRPGAuraGameplayTags::Get().InputTag);

		// 成功获取OldSpec说明这个NewInputSlot插槽位置上有已经装备的能力,需要交换标签并且广播这个OldSpec对应的abilityInfo
		if (OldSpec)
		{
			bIsSwapAbilitySlotPos = true;
			OldAbilityInfo        = GetMyGiSystem()->GetAbilityInfoAsset()->FindAbilityInfo(
			 GetTagFromAbilitySpec(*OldSpec, CurrentEquipAbilityAreaType));
			AddTagToAbilitySpecContainer(OldSpec->DynamicAbilityTags,
			                             OldInputSlot,
			                             FRPGAuraGameplayTags::Get().InputTag);
		}
	}
	else { return; }

	if (OldSpec) { MarkAbilitySpecDirty(*OldSpec); }

	MarkAbilitySpecDirty(*ToEquipAbilitySpec);

	// TODO 被动能力装备获取卸下来时的启用和停用 
	ClientEquipAbility(NewAbilityInfo,
	                   OldAbilityInfo,
	                   CurrentEquipAbilityAreaType,
	                   NewInputSlot,
	                   OldInputSlot,
	                   bIsSwapAbilitySlotPos);
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
	// 只广播一次,而不是每次OnRep都广播
	if (!BIsBroadCastedDefault)
	{
		// TODO UI控制器里调用以下两个方法不生效 待修复(只能在OnRep里调用生效)  
		BroadCastDefaultActivatableAbilitiesInfo();
		BroadCastDefaultSpellButtonAbilitiesInfo();
		BIsBroadCastedDefault = true;
		UE_LOG(UBaseAbilitySystemComponentLog,
		       Warning,
		       TEXT("[%s]->ActivateAbilities网络复制!"),
		       *FString::Printf(TEXT(__FUNCTION__)));
	}
}

void UBaseAbilitySystemComponent::ClientOnSpellButtonAbilityInfoChange_Implementation(const FTagToAbilityInfo& Info)
{
	if (!GetMyGiSystem()) { return; }

	GetMyGiSystem()->OnSpellButtonAbilityInfoChange.Broadcast(Info);
}

void UBaseAbilitySystemComponent::ClientOnAbilityStatusChanged_Implementation(
	const FGameplayTag& AbilityTag,
	const FGameplayTag& AbilityStatusTag,
	int32               AbilityLevel) { OnAbilityStatusChanged.Broadcast(AbilityTag, AbilityStatusTag, AbilityLevel); }

void UBaseAbilitySystemComponent::ClientOnGEAppliedToSelf_Implementation(
	UAbilitySystemComponent*    AbilitySystemComponent,
	const FGameplayEffectSpec&  GameplayEffectSpec,
	FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// UE_LOG(UBaseAbilitySystemComponentLog, Warning, TEXT("[%s]In , GeSpec[%s]"),*GetOwner()->GetName(),*GameplayEffectSpec.Def.GetName());


	FGameplayTagContainer Tags;


	GameplayEffectSpec.GetAllAssetTags(Tags);

	if (Tags.Num()) { OnGetAssetTagsDelegate.Broadcast(Tags); }
}

void UBaseAbilitySystemComponent::ClientEquipAbility_Implementation(
	const FTagToAbilityInfo& NewAbilityInfo,
	const FTagToAbilityInfo& OldAbilityInfo,
	const FGameplayTag&      EquipAbilityAreaType,
	const FGameplayTag&      NewInputSlot,
	const FGameplayTag&      OldInputSlot,
	const bool               bIsSwapAbilitySlot)
{
	if (!GetMyGiSystem()) { return; }

	// 广播新的能力信息(法术菜单的技能装备栏以及主界面的技能栏)
	if (NewAbilityInfo.InfoDataInputIsValid()) { GetMyGiSystem()->AbilityInfoDelegate.Broadcast(NewAbilityInfo); }

	// 法术菜单栏中的技能树按钮信息也需要更新
	if (NewAbilityInfo.InfoDataAbilityIsValid()) { ClientOnSpellButtonAbilityInfoChange(NewAbilityInfo); }


	GetMyGiSystem()->OnAbilityEquippedChange.Broadcast(NewAbilityInfo.AbilityTag,
	                                                   NewAbilityInfo.StatusTag,
	                                                   NewInputSlot,
	                                                   OldInputSlot,
	                                                   false);

	// 两个技能位交换时,需要给交换位置后的旧技能槽进行广播更新显示
	if (bIsSwapAbilitySlot)
	{
		auto LocalOldAbilityInfo      = OldAbilityInfo;
		LocalOldAbilityInfo.InputTag  = OldInputSlot;
		LocalOldAbilityInfo.StatusTag = FRPGAuraGameplayTags::Get().Abilities_Status_Equipped;
		if (LocalOldAbilityInfo.InfoDataInputIsValid())
		{
			GetMyGiSystem()->AbilityInfoDelegate.Broadcast(LocalOldAbilityInfo);
		}
		if (LocalOldAbilityInfo.InfoDataAbilityIsValid()) { ClientOnSpellButtonAbilityInfoChange(LocalOldAbilityInfo); }
		GetMyGiSystem()->OnAbilityEquippedChange.Broadcast(LocalOldAbilityInfo.AbilityTag,
		                                                   LocalOldAbilityInfo.StatusTag,
		                                                   OldInputSlot,
		                                                   NewInputSlot,
		                                                   bIsSwapAbilitySlot);
	}
}

bool UBaseAbilitySystemComponent::RemoveTagFromTagContainer(
	FGameplayTagContainer& TagContainer,
	const FGameplayTag&    TagToRemove)
{
	for (auto& Tag : TagContainer)
	{
		if (Tag.MatchesTag(TagToRemove))
		{
			TagContainer.RemoveTag(Tag);
			return true;
		}
	}
	return false;
}

void UBaseAbilitySystemComponent::AddTagToAbilitySpecContainer(
	FGameplayTagContainer& TagContainer,
	const FGameplayTag     TagToAdded,
	const FGameplayTag&    RemovedTag)
{
	if (TagContainer.HasTagExact(TagToAdded)) { return; }

	if (RemovedTag.IsValid()) { RemoveTagFromTagContainer(TagContainer, RemovedTag); }

	TagContainer.AddTag(TagToAdded);
}
