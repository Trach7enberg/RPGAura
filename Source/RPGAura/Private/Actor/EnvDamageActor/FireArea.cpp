// Copyright GGBAO 


#include "Actor/EnvDamageActor/FireArea.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/BoxComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"

// Sets default values
AFireArea::AFireArea()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	SetRootComponent(BoxComponent);
	PrimaryActorTick.bCanEverTick = false;
	ActorLevel = 1.f;
	bEnableEffectsToEnemies = true;
	OverlapThenDestroy = false;
}

void AFireArea::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AFireArea::OnBeginOverBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AFireArea::OnEndOverBegin);
}


void AFireArea::ClientApplyGEToTarget(AActor* Actor, TSubclassOf<UGameplayEffect> GeClass)
{
	if (!bEnableEffectsToEnemies && Actor->ActorHasTag(FRPGAuraGameplayTags::Get().Enemy)) { return; }

	UAbilitySystemComponent* ActorAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Actor);
	if (!ActorAsc || !GeClass) { return; }

	// Handle是GE上下文的一个包装,提供方便的操作,核心数据是一个叫Data的变量,这个就是实际的GE上下文
	FGameplayEffectContextHandle EffectContextHandle = ActorAsc->MakeEffectContext();

	// GE上下文可以存储与当前Gameplay effect有关的内容,比如是什么对象导致的这个GE,所以方便后面使用
	// 为GE上下文添加一个源对象,
	EffectContextHandle.AddSourceObject(this);


	// 从ASC 创建的上下文包装变量 获取 GE Spec handle
	const FGameplayEffectSpecHandle GeSpec = ActorAsc->MakeOutgoingSpec(GeClass, ActorLevel, EffectContextHandle);


	const auto ActiveGameplayEffectHandle = ActorAsc->ApplyGameplayEffectSpecToSelf(*GeSpec.Data.Get());
	ActiveGeMap.FindOrAdd(ActorAsc).Add(ActiveGameplayEffectHandle);
}

void AFireArea::OnBeginOverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	ClientApplyGEToTarget(OtherActor, CurrentGameplayEffectClass);
}

void AFireArea::OnEndOverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto TempAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!TempAsc) { return; }

	if (ActiveGeMap.Contains(TempAsc))
	{
		const auto s = ActiveGeMap[TempAsc];
		for (const auto& ActiveGeHandle : s)
		{
			// 移除GE,并且减少一层当前GE的堆叠层数
			// 不设置StacksToRemove层数的话会导致离开触发区域后再次遇到堆叠会触发不了堆叠效果
			TempAsc->RemoveActiveGameplayEffect(ActiveGeHandle, 1);
		}
		ActiveGeMap.FindAndRemoveChecked(TempAsc);
	}

	if (OverlapThenDestroy && IsValid(this)) { Destroy(); }
}
