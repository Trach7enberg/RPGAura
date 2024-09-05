// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "BaseAbilitySystemComponent.generated.h"

class FAbilityInfoSignature;
// 在GAS当中GE应用到玩家身上,并且获取资产标签时的委托
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGetAssetTagsDelegate, const FGameplayTagContainer& /* AssetTags */);


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

	/// 给玩家添加默认能力,可能有多个
	/// @param Abilities 能力列表
	/// @param CharacterLevel
	void AddCharacterDefaultAbilities(const TArray<TSubclassOf<UGameplayAbility>>& Abilities, float CharacterLevel = 1);

	/// 给玩家添加一个默认能力(通过能力类上的StarUp标签来给予)
	/// @param AbilityClass 能力类
	/// @param CharacterLevel
	void AddCharacterDefaultAbility(const TSubclassOf<UGameplayAbility>& AbilityClass, float CharacterLevel = 1);

	/// 处理InputAction按下时的能力触发问题
	/// @param InputTag 输入的游戏标签
	void AbilityInputTagPressed(const FGameplayTag& InputTag);

	/// 处理InputAction持续按下时的能力触发问题
	/// @param InputTag 输入的游戏标签
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	/// 处理InputAction按键释放时的能力问题
	/// @param InputTag 输入的游戏标签
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	/// 根据游戏标签激活能力 (能力的StarUpTag要有和参数Tag一样的标签才会激活)
	/// @param Tag 标签
	void TryActivateAbilityByTag(const FGameplayTag& Tag);

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

	/// 广播当前能被激活的(角色默认就有的)能力相关的信息数据
	void BroadCastDefaultActivatableAbilitiesInfo();

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
};
