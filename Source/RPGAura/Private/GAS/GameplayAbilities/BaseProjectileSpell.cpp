// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseProjectileSpell.h"

#include "GAS/AbilityTasks/TargetDataUnderCursor.h"
#include "Interfaces/CombatInterface.h"
#include "Weapons/Projectiles/BaseProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseProjectileSpellLog, All, All);

void UBaseProjectileSpell::SpawnProjectile(const FHitResult HitResult) const
{
	// 飞弹能力(投射物)仅在服务器上生成
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) { return; }

	if (!ProjectileClass)
	{
		UE_LOG(UBaseProjectileSpellLog, Error, TEXT("Projectile Class不能为nullptr!"));
		return;
	}
	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo())
	{
		UE_LOG(UBaseProjectileSpellLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}

	// Instigate : 引发者就是引发我们正在进行的事情的角色 , 必须是Pawn
	const auto Instigate = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!Instigate) { return; }

	const auto CombatInter = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInter)
	{
		UE_LOG(UBaseProjectileSpellLog, Error, TEXT("转换失败!"));
		return;
	}

	// TODO 设置飞弹的Rotation
	FTransform Transform;
	Transform.SetLocation(CombatInter->GetCombatSocketLocation());
	Transform.SetRotation((HitResult.ImpactPoint - CombatInter->GetCombatSocketLocation()).Rotation().Quaternion());

	const auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
		ProjectileClass,
		Transform,
		GetOwningActorFromActorInfo(),
		Instigate,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// TODO 给飞弹一个GameplayEffectSpec , 可以让它在overlap的时候应用

	if (GetCurrentActorInfo()->PlayerController.Get())
	{
		GetCurrentActorInfo()->PlayerController.Get()->SetControlRotation(Transform.Rotator());
	}
	Projectile->FinishSpawning(Transform);
}

void UBaseProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// 飞弹能力(投射物)仅在服务器上生成
	if (!HasAuthority(&ActivationInfo)) { return; }


	// SpawnProjectile();
}
