// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "Interfaces/CombatInterface.h"
#include "Weapons/Projectiles/BaseProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseProjectileAbilityLog, All, All);


UBaseProjectileAbility::UBaseProjectileAbility() {}

void UBaseProjectileAbility::SpawnProjectile(const FHitResult& HitResult,
                                             const FGameplayTag SocketAssociatedWithMontageTag)
{
	// 飞弹能力(投射物)仅在服务器上生成 , 世界正在摧毁也不能生成
	if (!GetAvatarActorFromActorInfo()->HasAuthority() || !GetWorld()) { return; }

	if (!ProjectileClass || !DamageEffectClass)
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("GE Class不能为nullptr!"));
		return;
	}
	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}

	// Instigate : 引发者就是引发我们正在进行的事情的角色 , 必须是Pawn
	const auto Instigate = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!Instigate) { return; }

	const auto CombatInter = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInter)
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("转换失败!"));
		return;
	}

	FTransform Transform;
	const auto AttackSocketLoc = CombatInter->GetCombatSocketLocation(SocketAssociatedWithMontageTag);
	Transform.SetLocation(AttackSocketLoc);

	const FRotator Rotation = (HitResult.ImpactPoint - AttackSocketLoc).Rotation();

	// 飞弹的倾斜度是与地面平行
	// Rotation.Pitch = 0.f;
	Transform.SetRotation(Rotation.Quaternion());


	const auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
		ProjectileClass,
		Transform,
		GetAvatarActorFromActorInfo(),
		Instigate,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	if (GetCurrentActorInfo()->PlayerController.Get())
	{
		GetCurrentActorInfo()->PlayerController.Get()->SetControlRotation(Transform.Rotator());
	}

	if (IsValid(Projectile))
	{
		// 创建GE
		MakeDamageEffectParamsFromAbilityDefaults(Projectile->DamageEffectParams);
		// 完成飞弹生成
		Projectile->FinishSpawning(Transform);
	}
	else { UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("[%s]生成飞弹失败!"), *GetName()); }
}

void UBaseProjectileAbility::UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel)
{
	if (!CurrentAbilityDescription.IsDescriptionValid()) { return; }
	const auto DescStrNormal = CurrentAbilityDescription.DescriptionNormal.ToString();
	const auto DescStrLocked = CurrentAbilityDescription.DescriptionLocked.ToString();
	const auto DescStrNextLevel = CurrentAbilityDescription.DescriptionNextLevel.ToString();

	const auto FormatNormalStr = FString::Format(*DescStrNormal, {
		                                             // 多少枚投射物
		                                             AbilityLevel,
		                                             // 预计伤害
		                                             FString::Printf(
			                                             TEXT("%.1f"),
			                                             GetEstimatedDamageFromDamageTypesMap(AbilityLevel)),
		                                             // 能力蓝耗值
		                                             FString::Printf(
			                                             TEXT("%.1f"),
			                                             FMath::Abs(GetManaCost(AbilityLevel))),
		                                             // 能力冷却时间
		                                             FString::Printf(TEXT("%.1f"), GetCoolDown(AbilityLevel)),
		                                             // 当前技能等级
		                                             AbilityLevel
	                                             });

	const auto NextLevel = AbilityLevel + 1;
	const auto FormatNextLevelStr = FString::Format(*DescStrNextLevel, {
		                                                // 多少枚投射物
		                                                NextLevel,
		                                                // 预计伤害
		                                                FString::Printf(
			                                                TEXT("%.1f"),
			                                                GetEstimatedDamageFromDamageTypesMap(NextLevel)),
		                                                // 能力蓝耗值
		                                                FString::Printf(
			                                                TEXT("%.1f"),
			                                                FMath::Abs(GetManaCost(NextLevel))),
		                                                // 能力冷却时间
		                                                FString::Printf(TEXT("%.1f"), GetCoolDown(NextLevel)),
		                                                // 当前技能等级
		                                                NextLevel
	                                                });
	CurrentAbilityDescription.DescriptionNormal = FText::FromString(FormatNormalStr);
	CurrentAbilityDescription.DescriptionNextLevel = FText::FromString(FormatNextLevelStr);
}


void UBaseProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}
