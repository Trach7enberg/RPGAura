// Copyright GGBAO 


#include "Pickup/BasePickup.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"

ABasePickup::ABasePickup()
{
	PrimaryActorTick.bCanEverTick = false;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetRelativeScale3D(FVector(0.3f));

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetRelativeLocation(FVector(0,13.333333,36.666665));
	SphereComponent->InitSphereRadius(140.0f);
	SphereComponent->SetHiddenInGame(false);

	ActorLevel = 1.f;
	bEnableEffectsToEnemies = false;
	PickupThenDestroy = true;
}


void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnBeginOverBegin);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ABasePickup::OnEndOverBegin);
}

void ABasePickup::ClientApplyGEToTarget(AActor* Actor, TSubclassOf<UGameplayEffect> GeClass)
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


	const FActiveGameplayEffectHandle ActiveGeHandle = ActorAsc->ApplyGameplayEffectSpecToSelf(*GeSpec.Data.Get());

	const auto BIsInfiniteActive = GeSpec.Data.Get()->Def.Get()->DurationPolicy ==
		EGameplayEffectDurationType::Infinite;

	// 效果是无限的才需要保存,用于之后移除效果
	if (BIsInfiniteActive) { CurrentActiveGameplayEffectHandle = ActiveGeHandle; }

	if (PickupThenDestroy && !BIsInfiniteActive) { Destroy(); }
}

void ABasePickup::OnBeginOverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	ClientApplyGEToTarget(OtherActor, CurrentGameplayEffectClass);
}

void ABasePickup::OnEndOverBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	const auto TempAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (TempAsc && CurrentActiveGameplayEffectHandle.IsValid())
	{
		TempAsc->RemoveActiveGameplayEffect(CurrentActiveGameplayEffectHandle, 1);
		if (IsValid(this)) { Destroy(); }
	}
}
