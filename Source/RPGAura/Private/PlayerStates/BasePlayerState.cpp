// Copyright GGBAO 


#include "PlayerStates/BasePlayerState.h"

#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "GAS/AttributeSet/BaseAttributeSet.h"
#include "GAS/Data/LevelUpInfoAsset.h"
#include "Net/UnrealNetwork.h"
#include "SubSystems/RPGAuraGameInstanceSubsystem.h"

ABasePlayerState::ABasePlayerState()
{
	// 当玩家状态在服务器上发生变化时,服务器就会发送更新到所有客户端 这个值就是设置服务器发送更新到所有客户端的频率
	// 但是当玩家状态有GAS系统的组件时,这个值要设置高(慢)一点
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UBaseAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* ABasePlayerState::GetAbilitySystemComponent() const { return AbilitySystemComponent; }

void ABasePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABasePlayerState, PlayerLevel);
	DOREPLIFETIME(ABasePlayerState, PlayerXP);
	DOREPLIFETIME(ABasePlayerState, AssignableAttributePoints);
	DOREPLIFETIME(ABasePlayerState, AssignableSpellPoints);
}

void ABasePlayerState::SetPlayerXP(const int32 NewXp)
{
	// TODO 待设置Clamp
	PlayerXP = NewXp;
	PlayerXpChangeDelegate.Broadcast(PlayerXP);
}

void ABasePlayerState::AddToPlayerXP(const int32 AddedXp)
{
	if (!GetGiSubSystem()) { return; }
	const auto MaxXp = GetGiSubSystem()->GetCharacterDefaultMaxXP();
	if (GetPlayerCurrentXP() == MaxXp) { return; }
	PlayerXP = FMath::Clamp(PlayerXP + AddedXp, 0, MaxXp);
	PlayerXpChangeDelegate.Broadcast(PlayerXP);
}

void ABasePlayerState::AddToAssignableAttributePoints(const int32 AddedValue)
{
	AssignableAttributePoints = FMath::Max(AssignableAttributePoints + AddedValue, 0);
	AssignableAttributePointsChangeDelegate.Broadcast(AssignableAttributePoints);
}

int32 ABasePlayerState::GetMaximumXPofLevel(const int32 TLevel)
{
	if (!GetGiSubSystem() || !GetGiSubSystem()->LevelUpInfoAsset) { return 0; }
	return GetGiSubSystem()->LevelUpInfoAsset->GetMaximumXPofLevel(TLevel);
}

int32 ABasePlayerState::GetMinimumXpRequiredForLevel(const int32 TLevel)
{
	if (!GetGiSubSystem() || !GetGiSubSystem()->LevelUpInfoAsset) { return 0; }

	return GetGiSubSystem()->LevelUpInfoAsset->GetMinimumXpRequiredForLevel(TLevel);
}

int32 ABasePlayerState::GetAttributePointsReward(const int32 CharacterLevel, const int32 MultipleLevel)
{
	if (!GetGiSubSystem() || !GetGiSubSystem()->LevelUpInfoAsset || CharacterLevel >= GetGiSubSystem()->LevelUpInfoAsset
		->LevelUpInfos.Num()) { return 0; }

	auto Result = GetGiSubSystem()->LevelUpInfoAsset->LevelUpInfos[CharacterLevel].AttributedPointAward;
	for (int j = MultipleLevel; j > 1; --j)
	{
		Result += GetGiSubSystem()->LevelUpInfoAsset->LevelUpInfos[CharacterLevel - j].AttributedPointAward;
	}
	return Result;
}

int32 ABasePlayerState::GetSpellPointsReward(const int32 CharacterLevel, int32 MultipleLevel)
{
	if (!GetGiSubSystem() || !GetGiSubSystem()->LevelUpInfoAsset || CharacterLevel >= GetGiSubSystem()->LevelUpInfoAsset
		->LevelUpInfos.Num()) { return 0; }

	auto Result = GetGiSubSystem()->LevelUpInfoAsset->LevelUpInfos[CharacterLevel].SpellPointAward;
	for (int j = MultipleLevel; j > 1; --j)
	{
		Result += GetGiSubSystem()->LevelUpInfoAsset->LevelUpInfos[CharacterLevel - j].SpellPointAward;
	}
	return Result;
}

void ABasePlayerState::OnRep_PlayerLevel(int32 OldValue) { PlayerLevelChangeDelegate.Broadcast(PlayerLevel); }

void ABasePlayerState::OnRep_PlayerXP(int32 OldValue)
{
	// 当PlayerXP被复制到客户端时,应该广播该值,以便HUD响应
	PlayerXpChangeDelegate.Broadcast(PlayerXP);
}

void ABasePlayerState::OnRep_AssignableAttributePoints(int32 OldValue)
{
	AssignableAttributePointsChangeDelegate.Broadcast(AssignableAttributePoints);
}
void ABasePlayerState::OnRep_AssignableSpellPoints(int32 OldValue)
{
	AssignableSpellPointsChangeDelegate.Broadcast(AssignableSpellPoints);
}

URPGAuraGameInstanceSubsystem* ABasePlayerState::GetGiSubSystem()
{
	if (!MyGiSubSystem)
	{
		if (!GetGameInstance()) { return nullptr; }
		MyGiSubSystem = Cast<URPGAuraGameInstanceSubsystem>(
			GetGameInstance()->GetSubsystem<URPGAuraGameInstanceSubsystem>());
	}
	return MyGiSubSystem;
}
