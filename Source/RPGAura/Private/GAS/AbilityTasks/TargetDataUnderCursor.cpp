// Copyright GGBAO 


#include "GAS/AbilityTasks/TargetDataUnderCursor.h"

DEFINE_LOG_CATEGORY_STATIC(UTargetDataUnderCursorLog, All, All);

UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderCursorProxy(UGameplayAbility* OwningAbility)
{
	const auto Result = NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);
	return Result;
}

void UTargetDataUnderCursor::Activate()
{
	if (Ability == nullptr) { return; }

	if (auto Asc = AbilitySystemComponent.Get())
	{
		auto ActorInfo = Ability->GetCurrentActorInfo();
		if (!ActorInfo) { return; }

		if (!ActorInfo->PlayerController.Get()) { return; }

		const auto Pc = ActorInfo->PlayerController.Get();
		FHitResult HitResult;
		Pc->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
		if (HitResult.bBlockingHit) { OnGetTargetDataUnderCursor.Broadcast(HitResult.ImpactPoint); }
	}
}
