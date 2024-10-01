// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

struct FAbilityDescription;
class FAbilityInfoSignature;
// 在GAS当中GE应用到玩家身上,并且获取资产标签时的委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetAssetTagsDelegate, const FGameplayTagContainer& /* AssetTags */);

// 能力的状态改变时的广播
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilityStatusChanged, const FGameplayTag& /* AbilityTag */,
                                       const FGameplayTag& /* AbilityStatusTag */, int32 /*AbilityLevel*/);


/**
 * ASC
 */
UCLASS()
class RPGAURA_API UBaseAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UBaseAbilitySystemComponent();

	virtual void BeginPlay() override;

	/// 初始化ASC中的一些设置、比如绑定回调函数
	virtual void InitSetting();

	// 应用GE时候 广播资产标签,用于弹出拾取信息
	FOnGetAssetTagsDelegate OnGetAssetTagsDelegate;

	// 应用于一个(角色的主动被动)能力的状态变化时
	FOnAbilityStatusChanged OnAbilityStatusChanged;

	/// 给玩家添加默认能力,可能有多个
	/// @param Abilities 能力列表
	/// @param CharacterLevel
	/// @param ActiveWhenGive
	void AddCharacterDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, float CharacterLevel = 1,
	                                  const bool ActiveWhenGive = false);

	/// 给玩家添加一个默认能力(通过能力类上的StarUp标签来给予)
	/// @param AbilityClass 能力类
	/// @param CharacterLevel
	/// @param ActiveWhenGive 给予能力时是否激活能力
	void AddCharacterDefaultAbility(const TSubclassOf<UGameplayAbility>& AbilityClass, float CharacterLevel = 1,
	                                bool ActiveWhenGive = false);

	/// 处理InputAction按下时的能力触发问题
	/// @param InputTag 输入的游戏标签
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	/// 处理InputAction持续按下时的能力触发问题
	/// @param InputTag 输入的游戏标签
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	/// 处理InputAction按键释放时的能力问题
	/// @param InputTag 输入的游戏标签
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/// 根据游戏标签激活能力 (能力的DefaultInputTag要有和参数Tag一样的标签才会激活)
	/// @param Tag 标签
	void TryActivateAbilityByDefaultInputTag(const FGameplayTag& Tag);

	/// 从AbilitySpec中获取包含有TargetTag标签的标签
	/// 获取到的结果是能力类中AbilityTags容器里的标签
	/// @param AbilitySpec 
	/// @param TargetTag 
	/// @return 
	static FGameplayTag GetTagFromAbilitySpec(const FGameplayAbilitySpec& AbilitySpec, const FGameplayTag& TargetTag);

	/// 从AbilitySpec的DynamicAbilityTags中获取包含有TargetTag标签的标签
	/// (DynamicAbilityTags容器保存有我们能力类中的InputTag)
	/// @param AbilitySpec 
	/// @param TargetTag 
	/// @return 
	static FGameplayTag GetTagFromAbilitySpecDynamicTags(const FGameplayAbilitySpec& AbilitySpec,
	                                                     const FGameplayTag& TargetTag);

	/// 从AbilitySpec中获取Abilities Status标签
	/// @param AbilitySpec 
	/// @return 
	static FGameplayTag GetAbilityStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	/// 根据能力标签返回与之对应的AbilitySpec,没有找到对应的Spec说明我们还没有这个标签对应的能力则返回nullptr
	/// @param AbilityTag 
	/// @return 
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	/// 当角色升级时,给予角色给定等级能启用的所有能力并且更新该能力的状态到Widget
	/// @param Level 给定的等级
	void UpdateAbilityStatusWhenLevelUp(const int32 Level);

	/// 广播当前能被激活的(角色默认就有的)能力相关的信息数据
	void BroadCastDefaultActivatableAbilitiesInfo();

	/// 广播默认法术菜单上的技能按钮所需要的信息
	void BroadCastDefaultSpellButtonAbilitiesInfo();
	
	/// 在服务器端根据属性标签升级对应的属性
	/// @param AttributeTag 与属性值一一对应的标签
	UFUNCTION(Server, Reliable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

	/// 在服务端根据能力标签消耗法术点
	/// @param AbilityTag 与能力一一对应的能力标签
	UFUNCTION(Server, Reliable)
	void UpgradeSpellPoint(const FGameplayTag& AbilityTag);

	/// 通过能力标签获取对应能力的详细描述
	/// @param AbilityTag  
	/// @return 
	FAbilityDescription GetAbilityDescriptionByAbilityTag(const FGameplayTag& AbilityTag);

protected:
	/// 当前ACS被应用任意的GE到自己身上时触发的回调函数
	/// @param AbilitySystemComponent 
	/// @param GameplayEffectSpec 
	/// @param ActiveEffectHandle 
	UFUNCTION(Client, Reliable)
	void ClientOnGEAppliedToSelf(UAbilitySystemComponent* AbilitySystemComponent,
	                             const FGameplayEffectSpec& GameplayEffectSpec,
	                             FActiveGameplayEffectHandle ActiveEffectHandle);

	/// 该函数会在GiveAbility后被调用(对ActivateAbilities进行网络复制)
	virtual void OnRep_ActivateAbilities() override;

	/// 角色的能力状态改变时候的广播
	/// @param AbilityTag 
	/// @param AbilityStatusTag
	/// @param AbilityLevel 
	UFUNCTION(Client, Reliable)
	void ClientOnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& AbilityStatusTag,
	                                  int32 AbilityLevel);

	/// 往给定容器里添加能力状态标签,能力状态标签同一时刻只能有一个,如果有相同的则不添加
	/// @param TagContainer 给定的标签容器
	/// @param AbilityStatusTag
	/// @param RemovedTag 需要在TagContainer中移除的标签
	void AddAbilityStatusTagToTagContainer(FGameplayTagContainer& TagContainer, const FGameplayTag AbilityStatusTag,
	                                       const FGameplayTag& RemovedTag = FGameplayTag{});

	/// 通过(AbilityTag)更新指定能力状态
	/// @param AbilityTag
	/// @param AbilityStatusTag
	/// @param AbilityLevel
	void UpdateAbilityStatus(const FGameplayTag& AbilityTag,const FGameplayTag& AbilityStatusTag, int32 AbilityLevel);

};
