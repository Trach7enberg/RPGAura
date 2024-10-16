// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseProjectileAbility.h"

#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraCoreTypes.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interfaces/CombatInterface.h"
#include "Weapons/Projectiles/BaseProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseProjectileAbilityLog, All, All);


UBaseProjectileAbility::UBaseProjectileAbility()
{
	KnockBackChance = 10.f;
	NonHomingTarget = CreateDefaultSubobject<USceneComponent>("NonHomingTarget");
}

void UBaseProjectileAbility::SpawnProjectile(const FHitResult& HitResult,
                                             const FGameplayTag SocketAssociatedWithMontageTag)
{
	// 飞弹能力(投射物)仅在服务器上生成 , 世界正在摧毁也不能生成
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->HasAuthority() || !GetWorld()) { return; }

	if (!GetOwningActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}

	// Instigator : 引发者就是引发我们正在进行的事情的角色 , 必须是Pawn
	const auto Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!Instigator) { return; }

	const auto CombatInter = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInter)
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("转换失败!"));
		return;
	}


	FTransform Transform;
	const auto AttackSocketLoc = CombatInter->GetCombatSocketLocation(SocketAssociatedWithMontageTag);
	Transform.SetLocation(AttackSocketLoc);

	// 射弹朝向
	const FRotator Rotation = (HitResult.ImpactPoint - AttackSocketLoc).Rotation();

	// 飞弹的倾斜度是与地面平行
	// Rotation.Pitch = 0.f;
	Transform.SetRotation(Rotation.Quaternion());


	// 完成飞弹生成
	if (const auto Projectile = CreateProjectile(Transform, Instigator)) { Projectile->FinishSpawning(Transform); }
}

void UBaseProjectileAbility::SpawnProjectiles(const FHitResult& HitResult,
                                              const FGameplayTag SocketAssociatedWithMontageTag, AActor* HomingActor,
                                              const bool bOverridePitch, float OverridePitch)
{
	// 飞弹能力(投射物)仅在服务器上生成,世界正在摧毁也不能生成
	if (!GetAvatarActorFromActorInfo() || !GetAvatarActorFromActorInfo()->HasAuthority() || !GetWorld()) { return; }

	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("Actor信息获取失败!"));
		return;
	}

	// Instigate : 引发者就是引发我们正在进行的事情的角色 , 必须是Pawn
	const auto Instigator = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!Instigator) { return; }

	const auto CombatInter = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!CombatInter)
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("转换失败!"));
		return;
	}

	FTransform Transform;
	const auto AttackSocketLoc = CombatInter->GetCombatSocketLocation(SocketAssociatedWithMontageTag);
	Transform.SetLocation(AttackSocketLoc);

	// 计算射弹朝向
	FRotator Rotation = (HitResult.ImpactPoint - AttackSocketLoc).Rotation();
	if (bOverridePitch) { Rotation.Pitch = OverridePitch; }

	const auto ForwardVector = Rotation.Vector();
	// TODO 待修复当射弹是homing时,homing目标消失(但是并未死亡,正在处于死亡动画)射弹会在空中转圈 
	const auto ProjNum = FMath::Max(1, GetValidProjectileNum(GetAbilityLevel()));
	TArray<FVector> Directions;
	URPGAuraBlueprintFunctionLibrary::GetVectorBySpread(SpawnSpread, 5, ForwardVector, Directions);
	for (auto& Dire : Directions)
	{
		FTransform SpawnTrans;
		SpawnTrans.SetLocation(AttackSocketLoc);

		auto Rot = Dire.Rotation();
		SpawnTrans.SetRotation(Rot.Quaternion());

		auto Projectile = CreateProjectile(SpawnTrans, Instigator);
		if (!Projectile) { continue; }

		auto ProjComp = Projectile->GetProjectileMovementComponent();
		if (!ProjComp) { continue; }
		ProjComp->ProjectileGravityScale = ProjectileGravity;
			if (bIsHomingProjectile && HomingActor && HomingActor->Implements<UCombatInterface>())
		{
			ProjComp->HomingTargetComponent = HomingActor->GetRootComponent();
		}
		else
		{
			NonHomingTarget->SetWorldLocation(HitResult.ImpactPoint);
			ProjComp->HomingTargetComponent = NonHomingTarget;
		}
		ProjComp->HomingAccelerationMagnitude = FMath::RandRange(HomingMinAcceleration, HomingMaxAcceleration);
		ProjComp->bIsHomingProjectile = bIsHomingProjectile;
		
		// 完成飞弹生成
		Projectile->FinishSpawning(SpawnTrans);
	}
}

void UBaseProjectileAbility::UpdateAbilityDescription(const FGameplayTag& AbilityTag, int32 AbilityLevel)
{
	if (!CurrentAbilityDescription.IsDescriptionValid()) { return; }
	const auto DescStrNormal = CurrentAbilityDescription.DescriptionNormal.ToString();
	const auto DescStrLocked = CurrentAbilityDescription.DescriptionLocked.ToString();
	const auto DescStrNextLevel = CurrentAbilityDescription.DescriptionNextLevel.ToString();

	// TODO 待Clamp投射物的最大值 
	const auto FormatNormalStr = FString::Format(*DescStrNormal, {
		                                             // 当前技能等级{0}
		                                             AbilityLevel,
		                                             // 能力冷却时间{1}
		                                             FString::Printf(TEXT("%.1f"), GetCoolDown(AbilityLevel)),
		                                             // 多少枚投射物{2}
		                                             GetValidProjectileNum(AbilityLevel),
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
		                                                GetValidProjectileNum(AbilityLevel) + 1,
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

int32 UBaseProjectileAbility::GetValidProjectileNum(const int32 AbilityLevel) 
{
	
	return FMath::Min(AbilityLevel, MaxProjectileNum);
}

ABaseProjectile* UBaseProjectileAbility::CreateProjectile(const FTransform& Transform, APawn* Instigator) const
{
	if (!ProjectileClass || !DamageEffectClass)
	{
		UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("GE Class不能为nullptr!"));
		return nullptr;
	}

	const auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
		ProjectileClass,
		Transform,
		GetAvatarActorFromActorInfo(),
		Instigator,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	// if (GetCurrentActorInfo()->PlayerController.Get())
	// {
	// 	GetCurrentActorInfo()->PlayerController.Get()->SetControlRotation(Transform.Rotator());
	// }

	if (IsValid(Projectile))
	{
		// 在GE context中传输当前冲击法线向量,以便属性集能获取进行相应操作
		// 冲击向量是以Projectile的前向向量为基础,再以右向量为基础旋转45度,即Pitch Rotation -45度
		Projectile->DamageEffectParams.ImpulseVector = Projectile->GetActorForwardVector().RotateAngleAxis(
			45.f, Projectile->GetActorRightVector());
		MakeDamageEffectParamsFromAbilityDefaults(Projectile->DamageEffectParams);
	}
	else { UE_LOG(UBaseProjectileAbilityLog, Error, TEXT("[%s]生成飞弹失败!"), *GetName()); }
	return Projectile;
}
