// Copyright GGBAO 


#include "Controller/BasePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "GameplayTagContainer.h"
#include "Characters/EnemyCharacter.h"
#include "Components/SplineComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "Input/Components/BaseEnhancedInputComponent.h"
#include "PlayerStates/BasePlayerState.h"

DEFINE_LOG_CATEGORY_STATIC(ABasePlayerControllerLog, All, All);

ABasePlayerController::ABasePlayerController()
{
    // 启用复制
    bReplicates = true;
    bShowMouseCursor = true;

    // 设置鼠标的样式
    DefaultMouseCursor = EMouseCursor::Type::Default;

    CurrentActor = nullptr;
    CurseHitResult = FHitResult();
    CachedDestination = FVector::ZeroVector;
    FollowCursorTime = 0.f;
    ShortPressThreshold = .5f;
    StopDistance = 50.f;
    BIsAutoRunning = false;
    BIsTargeting = false;

    SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void ABasePlayerController::BeginPlay()
{
    Super::BeginPlay();
    check(InputContext);

    // 获得增强输入的本地玩家子系统,通过这个根据添加我们的上下文映射
    const auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (!SubSystem)
    {
        UE_LOG(ABasePlayerControllerLog, Error, TEXT("本地玩家子系统为null!"));
        return;
    }

    // 添加输入上下文映射,由于可以添加多个,它们之间的优先级用优先级数字来表示
    SubSystem->AddMappingContext(InputContext, 0);

    // 输入模式,可以键盘和鼠标同时输入,并且输入可以影响UI
    FInputModeGameAndUI InputModeGameAndUI;
    // 鼠标不会被锁定到视口内,意味着可以移出视口
    InputModeGameAndUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputModeGameAndUI.SetHideCursorDuringCapture(false);
    SetInputMode(InputModeGameAndUI);
}

void ABasePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (!InputContext)
    {
        return;
    }

    // 输入组件转换为增强输入组件
    const auto Input = CastChecked<UBaseEnhancedInputComponent>(InputComponent);
    if (!Input)
    {
        UE_LOG(ABasePlayerControllerLog, Error, TEXT("输入组件转换为增强输入组件失败!"));
        return;
    }
    Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
    Input->BindAbilityActions(InputConfig, this, &ABasePlayerController::AbilityInputTagPressed, &ABasePlayerController::AbilityInputTagReleased, &ABasePlayerController::AbilityInputTagHeld);
}

void ABasePlayerController::Move(const FInputActionValue& InputActionValue)
{
    const auto InputAxisValue = InputActionValue.Get<FVector2D>();
    if (GetPawn())
    {
        GetPawn()->AddMovementInput(GetControlRotation().Euler().ForwardVector, InputAxisValue.Y);
        GetPawn()->AddMovementInput(GetControlRotation().Euler().RightVector, InputAxisValue.X);
    }
}

UBaseAbilitySystemComponent* ABasePlayerController::GetAbilitySystemComponent()
{
    if (!AbilitySystemComponent)
    {
        if (const auto Ps = GetPlayerState<ABasePlayerState>())
        {
            AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
            if (!AbilitySystemComponent)
            {
                UE_LOG(ABasePlayerControllerLog, Error, TEXT("在控制器中获得Asc失败"));
                return nullptr;
            }
        }
    }
    return AbilitySystemComponent;
}

void ABasePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
    if (!GetAbilitySystemComponent())
    {
        return;
    }

    const auto GameplayEnum = FRPGAuraGameplayTags::FindEnumByTag(InputTag);
    if (!GameplayEnum)
    {
        return;
    }

    GetHitResultUnderCursor(ECC_Visibility, false, CurseHitResult);

    // GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, *InputTag.ToString());
    switch (*GameplayEnum)
    {
        // 传进来的是鼠标左键点击的话,实现点击自动走路
        case EGameplayTagNum::InputLMB:
            
            if (CurseHitResult.bBlockingHit)
            {
                // 击中的是敌人,那么就是在瞄准,同时激活能力和取消奔跑
                CurrentActor = Cast<AEnemyCharacter>(CurseHitResult.GetActor());
                if (CurrentActor)
                {
                    BIsAutoRunning = false;
                    BIsTargeting = true;
                    GetAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
                }
                else
                {
                    BIsTargeting = false;
                }
            }

            break;
        default:

            break;
    }
}


void ABasePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
    if (!GetAbilitySystemComponent() || !GetWorld())
    {
        return;
    }

    const auto GameplayEnum = FRPGAuraGameplayTags::FindEnumByTag(InputTag);
    if (!GameplayEnum)
    {
        return;
    }

    GetHitResultUnderCursor(ECC_Visibility, false, CurseHitResult);

    /**
     * 
     */
    switch (*GameplayEnum)
    {
        case EGameplayTagNum::InputLMB:
            GetHitResultUnderCursor(ECC_Visibility, false, CurseHitResult);
            if (CurseHitResult.bBlockingHit)
            {
                CurrentActor = Cast<AEnemyCharacter>(CurseHitResult.GetActor());
                if (CurrentActor)
                {
                    BIsAutoRunning = false;
                    BIsTargeting = true;
                    GetAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
                }
                else
                {
                    BIsTargeting = false;
                    FollowCursorTime += GetWorld()->GetDeltaSeconds();
                    CachedDestination = CurseHitResult.ImpactPoint;
                    if (GetPawn())
                    {
                        GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, FString("Walking"));
                        const FVector WorldDire = (CachedDestination - GetPawn()->GetActorLocation()).GetSafeNormal();
                        GetPawn()->AddMovementInput(WorldDire);
                    }
                }
            }

            break;
        default:

            break;
    }

    // GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
}

void ABasePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
    if (!GetAbilitySystemComponent())
    {
        return;
    }
    GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);
}