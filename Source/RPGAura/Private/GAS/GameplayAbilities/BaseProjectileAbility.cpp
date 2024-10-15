// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "Interfaces/CombatInterface.h"
#include "Weapons/Projectiles/BaseProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseProjectileAbilityLog, All, All);


UBaseProjectileAbility::UBaseProjectileAbility() { KnockBackChance = 10.f; }

void UBaseProjectileAbility::SpawnProjectile(const FHitResult& HitResult,
                                             const FGameplayTag SocketAssociatedWithMontageTag)
{
	// 飞弹能力(投射物)仅在服务器上生成 , 世界正在摧毁也不能生成
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->HasAuthority() || !GetWorld()) { return; }

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
		// 在GE context中传输当前冲击法线向量,以便属性集能获取进行相应操作
		// 冲击向量是以Projectile的前向向量为基础,再以右向量为基础旋转45度,即Pitch Rotation -45度
		Projectile->DamageEffectParams.ImpulseVector = Projectile->GetActorForwardVector().RotateAngleAxis(
			45.f, Projectile->GetActorRightVector());
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
		                                             // 当前技能等级{0}
		                                             AbilityLevel,
		                                             // 能力冷却时间{1}
		                                             FString::Printf(TEXT("%.1f"), GetCoolDown(AbilityLevel)),
		                                             // 多少枚投射物{2}
		                                             AbilityLevel,
		                                             // 预计伤害{3}
		                                             FString::Printf(
			                                             TEXT("%.1f"),
			                                             GetEstimatedDamageFromDamageTypesMap(AbilityLevel)),
		                                             // DeBuff几率{4}
		                                             FString::Printf(TEXT("%.1f"), DeBuffChance),
		                                             // DeBuff持续时间{5}
		                                             FString::Printf(TEXT("%.1f"), DeBuffDuration),
		                                             // DeBuff伤害{6}
		                                             FString::Printf(TEXT("%.1f"), DeBuffDamage),
		                                             // 能力蓝耗值{7}
		                                             FString::Printf(
			                                             TEXT("%.1f"),
			                                             FMath::Abs(GetManaCost(AbilityLevel))),
	                                             });

	const auto NextLevel = AbilityLevel + 1;
	const auto FormatNextLevelStr = FString::Format(*DescStrNextLevel, {
													 // 当前技能等级{0}
													 NextLevel,
													 // 能力冷却时间{1}
													 FString::Printf(TEXT("%.1f"), GetCoolDown(NextLevel)),
													 // 多少枚投射物{2}
													 NextLevel,
													 // 预计伤害{3}
													 FString::Printf(
														 TEXT("%.1f"),
														 GetEstimatedDamageFromDamageTypesMap(NextLevel)),
													 // DeBuff几率{4}
													 FString::Printf(TEXT("%.1f"), DeBuffChance),
													 // DeBuff持续时间{5}
													 FString::Printf(TEXT("%.1f"), DeBuffDuration),
													 // DeBuff伤害{6}
													 FString::Printf(TEXT("%.1f"), DeBuffDamage),
													 // 能力蓝耗值{7}
													 FString::Printf(
														 TEXT("%.1f"),
														 FMath::Abs(GetManaCost(NextLevel))),
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
