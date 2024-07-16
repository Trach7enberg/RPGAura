// Copyright GGBAO 


#include "Dev/DevGEActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


ADevGEActor::ADevGEActor() { PrimaryActorTick.bCanEverTick = false; }

void ADevGEActor::BeginPlay() { Super::BeginPlay(); }

void ADevGEActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ADevGEActor::ApplyGEToTarget(AActor *Actor, TSubclassOf<UGameplayEffect> GeClass) const
{
	const auto ActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ActorAsc || !GeClass) { return; }

	// Handle是GE上下文的一个包装,提供方便的操作,核心数据是一个叫Data的变量,这个就是实际的GE上下文
	FGameplayEffectContextHandle EffectContextHandle = ActorAsc->MakeEffectContext();

	// GE上下文可以存储与当前Gameplay effect有关的内容,比如是什么对象导致的这个GE,所以方便后面使用
	// 为GE上下文添加一个源对象
	EffectContextHandle.AddSourceObject(this);

	// 从ASC 创建的上下文包装变量 获取 GE Spec handle
	const FGameplayEffectSpecHandle GeSpec = ActorAsc->MakeOutgoingSpec(GeClass, 1.f, EffectContextHandle);

	ActorAsc->ApplyGameplayEffectSpecToSelf(*GeSpec.Data.Get());


}
