// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilityTasks/TargetDataUnderCursor.h"
#include "Interfaces/CombatInterface.h"
#include "Weapons/Projectiles/BaseProjectile.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseProjectileSpellLog, All, All);

void UBaseProjectileSpell::SpawnProjectile(const FHitResult HitResult) const
{
	// 飞弹能力(投射物)仅在服务器上生成
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) { return; }

	if (!ProjectileClass || !DamageEffectClass)
	{
		UE_LOG(UBaseProjectileSpellLog, Error, TEXT("GE Class不能为nullptr!"));
		return;
	}
	if (!GetOwningActorFromActorInfo() || !GetAvatarActorFromActorInfo() || !GetAbilitySystemComponentFromActorInfo())
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

	FTransform Transform;
	Transform.SetLocation(CombatInter->GetCombatSocketLocation());

	FRotator Rotation = (HitResult.ImpactPoint - CombatInter->GetCombatSocketLocation()).Rotation();

	// 飞弹的倾斜度是与地面平行
	Rotation.Pitch = 0.f;
	Transform.SetRotation(Rotation.Quaternion());


	const auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(
		ProjectileClass,
		Transform,
		GetOwningActorFromActorInfo(),
		Instigate, 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	if (GetCurrentActorInfo()->PlayerController.Get())
	{
		GetCurrentActorInfo()->PlayerController.Get()->SetControlRotation(Transform.Rotator());
	}

	// 创建GE 上下文
	FGameplayEffectContextHandle EffectContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	
	// 设置GE上下文,添加相关信息
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	EffectContextHandle.AddHitResult(HitResult);

	// 创建GE
	auto GameplayEffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	// 分配一个键值对给SetByCaller,键是游戏标签,值是设定的Magnitude,到时候在GE蓝图中选择我们分配的标签,该GE就会应用我们这里设定的Magnitude值
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(GameplayEffectSpecHandle,
	                                                              FRPGAuraGameplayTags::Get().Abilities_Damage_FireBolt,
	                                                              ScalableDamage.GetValueAtLevel(GetAbilityLevel()));


	Projectile->DamageEffectSpecHandle = GameplayEffectSpecHandle;


	// 完成飞弹生成
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
