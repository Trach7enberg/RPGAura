// Copyright GGBAO 


#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "GAS/Data/CharacterClassInfo.h"

DEFINE_LOG_CATEGORY_STATIC(URPGAuraGameInstanceSubsystemLog, All, All);

void URPGAuraGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CharacterClassInfo = LoadObject<UCharacterClassInfo>(
		this,TEXT(
			"/Script/RPGAura.CharacterClassInfo'/Game/Blueprints/GAS/Data/DataAssets/DA_CharacterClassInfo.DA_CharacterClassInfo'"));
	if (!CharacterClassInfo) { UE_LOG(URPGAuraGameInstanceSubsystemLog, Error, TEXT("加载资产失败!!")); }
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

	if (!CharacterClassInfo->SecondaryAttributesEnemy || !CharacterClassInfo->VitalAttributes || !CharacterClassInfo->
		SecondaryAttributesPlayer)
	{
		UE_LOG(URPGAuraGameInstanceSubsystemLog, Error, TEXT("数据资产不完整!!"));
		return;
	}

	FGameplayEffectContextHandle GameplayEffectContextHandle = Asc->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(Asc->GetAvatarActor());


	// 创建主、次、vital的GE Spec Handle
	const auto PrimaryGeSpecHandle = Asc->MakeOutgoingSpec(
		CharacterClassInfo->FindClassDefaultInfo(CharacterClass).PrimaryAttributes, Level, GameplayEffectContextHandle);

	// NPC和玩家的的次要属性影响不一样
	const auto SecondaryGeSpecHandle = Asc->MakeOutgoingSpec(
		(BIsPlayer)
			? CharacterClassInfo->SecondaryAttributesPlayer
			: CharacterClassInfo->SecondaryAttributesEnemy, Level,
		GameplayEffectContextHandle);
	
	const auto VitalGeSpecHandle = Asc->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes, Level, GameplayEffectContextHandle);


	Asc->ApplyGameplayEffectSpecToSelf(*PrimaryGeSpecHandle.Data.Get());
	Asc->ApplyGameplayEffectSpecToSelf(*SecondaryGeSpecHandle.Data.Get());
	Asc->ApplyGameplayEffectSpecToSelf(*VitalGeSpecHandle.Data.Get());
}
