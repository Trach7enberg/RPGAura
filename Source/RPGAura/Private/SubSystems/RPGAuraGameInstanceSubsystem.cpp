// Copyright GGBAO 


#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

#include "AbilitySystemComponent.h"

#include "CoreTypes/RPGAuraGasCoreTypes.h"
#include "GAS/Data/CharacterClassInfo.h"
#include "GAS/Data/PickupMessageAsset.h"
#include "GAS/Data/TagToAbilityInfoAsset.h"

DEFINE_LOG_CATEGORY_STATIC(URPGAuraGameInstanceSubsystemLog, All, All);

void URPGAuraGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CharacterClassInfo = LoadObject<UCharacterClassInfo>(
		this,TEXT(
			"/Script/RPGAura.CharacterClassInfo'/Game/Blueprints/GAS/Data/DataAssets/DA_CharacterClassInfo.DA_CharacterClassInfo'"));


	MessageWidgetDataAsset = LoadObject<UPickupMessageAsset>(
		this,TEXT(
			"/Script/RPGAura.PickupMessageAsset'/Game/Blueprints/GAS/Data/DataAssets/DA_PickupMessage.DA_PickupMessage'"));

	AbilityInfoAsset = LoadObject<UTagToAbilityInfoAsset>(
		this,TEXT(
			"/Script/RPGAura.TagToAbilityInfoAsset'/Game/Blueprints/GAS/Data/DataAssets/DA_TagToAbilityInfo.DA_TagToAbilityInfo'"));

	if (!CharacterClassInfo)
	{
		UE_LOG(URPGAuraGameInstanceSubsystemLog, Error, TEXT("[%s]加载角色职业信息资产失败!!"), *GetNameSafe(this));
	}
	if (!MessageWidgetDataAsset)
	{
		UE_LOG(URPGAuraGameInstanceSubsystemLog, Error, TEXT("[%s]加载弹出消息数据资产表失败"), *GetNameSafe(this));
	}
	if (!AbilityInfoAsset)
	{
		UE_LOG(URPGAuraGameInstanceSubsystemLog, Error, TEXT("[%s]加载技能信息消息数据资产表失败"), *GetNameSafe(this));
	}
}

void URPGAuraGameInstanceSubsystem::InitializeDefaultAttributes(UAbilitySystemComponent* Asc,
                                                                ECharacterClass CharacterClass, float Level,
                                                                bool BIsPlayer)
{
	if (!CharacterClassInfo->CharacterClassInformation.Num() || !Asc || !Asc->GetAvatarActor())
	{
		UE_LOG(URPGAuraGameInstanceSubsystemLog, Error, TEXT("数据资产或者能力系统组件为nullptr!!"));
		return;
	}

	const auto CInfo = CharacterClassInfo->FindClassDefaultInfo(CharacterClass);

	if (!CharacterClassInfo->SecondaryAttributesEnemy || !CharacterClassInfo->VitalAttributes || !CharacterClassInfo->
		SecondaryAttributesPlayer || !CInfo.SecondaryResistanceAttributes)
	{
		UE_LOG(URPGAuraGameInstanceSubsystemLog, Error, TEXT("数据资产不完整!!"));
		return;
	}


	FGameplayEffectContextHandle GameplayEffectContextHandle = Asc->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(Asc->GetAvatarActor());


	// 创建主、次、vital的GE Spec Handle
	const auto PrimaryGeSpecHandle = Asc->MakeOutgoingSpec(
		CInfo.PrimaryAttributes, Level, GameplayEffectContextHandle);

	// NPC和玩家的的次要属性影响不一样
	const auto SecondaryGeSpecHandle = Asc->MakeOutgoingSpec(
		(BIsPlayer)
			? CharacterClassInfo->SecondaryAttributesPlayer
			: CharacterClassInfo->SecondaryAttributesEnemy, Level,
		GameplayEffectContextHandle);

	const auto SecondaryResistanceGeSpecHandle = Asc->MakeOutgoingSpec(
		CInfo.SecondaryResistanceAttributes, Level, GameplayEffectContextHandle);

	const auto VitalGeSpecHandle = Asc->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes, Level, GameplayEffectContextHandle);


	Asc->ApplyGameplayEffectSpecToTarget(*PrimaryGeSpecHandle.Data.Get(), Asc);
	Asc->ApplyGameplayEffectSpecToTarget(*SecondaryGeSpecHandle.Data.Get(), Asc);
	Asc->ApplyGameplayEffectSpecToTarget(*VitalGeSpecHandle.Data.Get(), Asc);
	Asc->ApplyGameplayEffectSpecToTarget(*SecondaryResistanceGeSpecHandle.Data.Get(), Asc);

	// TODO 未知BUG,单人模式时,敌人的次要属性部分无法初始化(比如格挡几率),服务器则正常初始化,但数值也有问题,因此只能多加一次调用,暂时查明为敌人GE蓝图类中的持续政策不是无限的原因 所以只能将政策改为infinite了
	// Asc->ApplyGameplayEffectSpecToSelf(*SecondaryGeSpecHandle.Data.Get());
}
