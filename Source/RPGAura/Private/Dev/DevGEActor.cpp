// Copyright GGBAO 


#include "Dev/DevGEActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(ADevGEActorLog, All, All);

ADevGEActor::ADevGEActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADevGEActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADevGEActor::ClientApplyGEToTarget(AActor *Actor, TSubclassOf<UGameplayEffect> GeClass)
{
	UAbilitySystemComponent *ActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ActorAsc || !GeClass)
	{
		return;
	}

	// Handle是GE上下文的一个包装,提供方便的操作,核心数据是一个叫Data的变量,这个就是实际的GE上下文
	FGameplayEffectContextHandle EffectContextHandle = ActorAsc->MakeEffectContext();

	// GE上下文可以存储与当前Gameplay effect有关的内容,比如是什么对象导致的这个GE,所以方便后面使用
	// 为GE上下文添加一个源对象
	EffectContextHandle.AddSourceObject(this);


	// 从ASC 创建的上下文包装变量 获取 GE Spec handle
	const FGameplayEffectSpecHandle GeSpec = ActorAsc->MakeOutgoingSpec(GeClass, ActorLevel, EffectContextHandle);
	
	
	const FActiveGameplayEffectHandle ActiveGeHandle = ActorAsc->ApplyGameplayEffectSpecToSelf(*GeSpec.Data.Get());

	const auto BIsActive = GeSpec.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

	if (BIsActive && InfinityRemovalPolicy == EGeRemovalPolicy::RemoveOnEndOverlap)
	{
		if (ActiveGeMap.Contains(ActorAsc))
		{
			ActiveGeMap[ActorAsc].Add(ActiveGeHandle);
		}
		else
		{
			ActiveGeMap.Add(ActorAsc, {ActiveGeHandle});
		}
	}
	
}

void ADevGEActor::OnOverLap(AActor *TargetActor)
{
	if (CurrentApplicationPolicy == EGeApplicationPolicy::ApplyOnOverlap)
	{
		ClientApplyGEToTarget(TargetActor, CurrentGameplayEffectClass);
	}
}

void ADevGEActor::EndOverLap(AActor *TargetActor, bool DestroyActor)
{
	if (CurrentApplicationPolicy == EGeApplicationPolicy::ApplyOnEndOverlap)
	{
		ClientApplyGEToTarget(TargetActor, CurrentGameplayEffectClass);
	}

	if (InfinityRemovalPolicy == EGeRemovalPolicy::RemoveOnEndOverlap)
	{
		const auto TempAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (ActiveGeMap.Contains(TempAsc))
		{
			const auto s = ActiveGeMap[TempAsc];
			for (const auto &ActiveGeHandle : s)
			{
				// 移除GE,并且减少一层当前GE的堆叠层数
				// 不设置StacksToRemove层数的话会导致离开触发区域后再次遇到堆叠会触发不了堆叠效果
				TempAsc->RemoveActiveGameplayEffect(ActiveGeHandle, 1);
			}
			ActiveGeMap.FindAndRemoveChecked(TempAsc);
		}
	}

	if (DestroyActor)
	{
		Destroy();
	}
}
