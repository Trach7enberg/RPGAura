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

	// 创建GE 上下文
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	// 设置GE上下文,添加相关信息
	EffectContextHandle.AddSourceObject(Projectile);


	// 创建GE
	const auto GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	// 分配SetByCaller
	AssignTagSetByCallerMagnitudeWithDamageType(GameplayEffectSpecHandle, GetAbilityLevel());

	if (Projectile && IsValid(Projectile))
	{
		Projectile->DamageEffectSpecHandle = GameplayEffectSpecHandle;
		// 完成飞弹生成
		Projectile->FinishSpawning(Transform);
	}
	else { UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("[%s]生成飞弹失败!"), *GetName()); }
}

void UBaseProjectileAbility::UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel)
{
	if (!CurrentAbilityDescription.IsDescriptionValid()) { return ; }
	const auto DescStrNormal = CurrentAbilityDescription.DescriptionNormal.ToString();
	const auto DescStrLocked = CurrentAbilityDescription.DescriptionLocked.ToString();
	const auto DescStrNextLevel = CurrentAbilityDescription.DescriptionNextLevel.ToString();

	const auto FormatNormalStr = FString::Format(*DescStrNormal, {
													 AbilityLevel,
													 FString::Printf(
														 TEXT("%.1f"),
														 GetEstimatedDamageFromDamageTypesMap(AbilityLevel)),
													 AbilityLevel
												 });
	const auto FormatNextLevelStr = FString::Format(*DescStrNextLevel, {
														AbilityLevel + 1,
														FString::Printf(
															TEXT("%.1f"),
															GetEstimatedDamageFromDamageTypesMap(AbilityLevel + 1)),
														AbilityLevel + 1,
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


