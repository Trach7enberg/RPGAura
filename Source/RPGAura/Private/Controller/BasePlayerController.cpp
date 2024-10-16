// Copyright GGBAO 


#include "Controller/BasePlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/EnemyCharacter.h"
#include "Components/SplineComponent.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "GAS/AbilitySystemComp/BaseAbilitySystemComponent.h"
#include "Input/Components/BaseEnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

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
	BIsAutoWalking = false;
	BIsTargeting = false;
	BIsShiftDown = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void ABasePlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	AutoWalking();
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	// 获得增强输入的本地玩家子系统,通过这个根据添加我们的上下文映射
	auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!SubSystem)
	{
		// TODO 在监听模式下 无法直接获得GetLocalPlayer(),因此这里暂时简单的获取一个控制器并设置子系统
		SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetLocalPlayer());
		if (!SubSystem)
		{
			UE_LOG(ABasePlayerControllerLog, Error, TEXT("[%s]的本地玩家子系统为null!"), *GetName());
			return;
		}
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
	if (!InputContext || !MoveAction || !ShiftAction) { return; }

	// 输入组件转换为增强输入组件
	const auto Input = CastChecked<UBaseEnhancedInputComponent>(InputComponent);
	if (!Input)
	{
		UE_LOG(ABasePlayerControllerLog, Error, TEXT("输入组件转换为增强输入组件失败!"));
		return;
	}
	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
	Input->BindAbilityActions(
		InputConfig,
		this,
		&ABasePlayerController::AbilityInputTagPressed,
		&ABasePlayerController::AbilityInputTagReleased,
		&ABasePlayerController::AbilityInputTagHeld);
	Input->BindAction(ShiftAction, ETriggerEvent::Triggered, this, &ABasePlayerController::ShiftPressed);
	Input->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ABasePlayerController::ShiftReleased);
}

void ABasePlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	// UE_LOG(ABasePlayerControllerLog, Warning, TEXT("控制器: [%s] , 以控制 : [%s] 角色"),*GetName(),*aPawn->GetName());
}

void ABasePlayerController::Move(const FInputActionValue& InputActionValue)
{
	BIsAutoWalking = false;
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
		AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
		if (!AbilitySystemComponent)
		{
			UE_LOG(ABasePlayerControllerLog, Error, TEXT("[%s]在控制器中获得Asc失败"), *GetName());
			return nullptr;
		}
	}
	return AbilitySystemComponent;
}

void ABasePlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (!GetAbilitySystemComponent()) { return; }


	const auto GameplayEnum = FRPGAuraGameplayTags::FindEnumByTag(InputTag);
	if (!GameplayEnum) { return; }

	GetHitResultUnderCursor(ECC_Visibility, false, CurseHitResult);

	// GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, *InputTag.ToString());
	switch (*GameplayEnum)
	{
	case EGameplayTagNum::InputLMB:
		if (CurseHitResult.bBlockingHit)
		{
			// 鼠标击中的是敌人,那么就是在瞄准,同时激活能力和取消奔跑
			CurrentActor = Cast<AEnemyCharacter>(CurseHitResult.GetActor());
			if (CurrentActor)
			{
				BIsAutoWalking = false;
				BIsTargeting = true;

				GetAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
			}
			else if (BIsShiftDown)
			{
				BIsAutoWalking = false;
				GetAbilitySystemComponent()->AbilityInputTagPressed(InputTag);
			}
			else
			{
				BIsTargeting = false;
			}
		}

		break;
	default: GetAbilitySystemComponent()->AbilityInputTagPressed(InputTag);

		break;
	}
}


void ABasePlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!GetAbilitySystemComponent() || !GetWorld()) { return; }

	const auto GameplayEnum = FRPGAuraGameplayTags::FindEnumByTag(InputTag);
	if (!GameplayEnum) { return; }

	GetHitResultUnderCursor(ECC_Visibility, false, CurseHitResult);


	switch (*GameplayEnum)
	{
	case EGameplayTagNum::InputLMB:
		if (CurseHitResult.bBlockingHit)
		{
			// 鼠标持续按下时断掉自动行走
			BIsAutoWalking = false;
			CurrentActor = Cast<AEnemyCharacter>(CurseHitResult.GetActor());

			// 如果鼠标击中的是敌人则触发能力
			if (CurrentActor)
			{
				BIsTargeting = true;
				GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
			}
			else if (BIsShiftDown)
			{
				BIsTargeting = false;
				GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
			}
			else
			{
				// 按住鼠标左键则移动 TODO 当点击走路的时候该分支最好一次都不执行,现在的情况是会执行一次然后进行点击移动的逻辑
				BIsTargeting = false;
				FollowCursorTime += GetWorld()->GetDeltaSeconds();
				CachedDestination = CurseHitResult.ImpactPoint;

				// 人物移动
				if (GetPawn())
				{
					const FVector WorldDire = (CachedDestination - GetPawn()->GetActorLocation()).GetSafeNormal();
					GetPawn()->AddMovementInput(WorldDire);
				}
			}
		}

		break;
	default:GetAbilitySystemComponent()->AbilityInputTagHeld(InputTag);
		break;
	}
}


void ABasePlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!GetAbilitySystemComponent() || !GetWorld()) { return; }

	const auto GameplayEnum = FRPGAuraGameplayTags::FindEnumByTag(InputTag);
	if (!GameplayEnum) { return; }

	GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);

	GetHitResultUnderCursor(ECC_Visibility, false, CurseHitResult);
	switch (*GameplayEnum)
	{
	// LMB释放
	case EGameplayTagNum::InputLMB:

		if (!BIsTargeting && !BIsShiftDown)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ClickVfx,CurseHitResult.ImpactPoint);
			CachedDestination = CurseHitResult.ImpactPoint;
			// 人物跟随鼠标的时间 小于短按阈值,则进行点按鼠标左键人物自动行走
			if (FollowCursorTime <= ShortPressThreshold && GetPawn())
			{
				// 创建导航路径,根据导航路径走
				UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(
					GetWorld(), GetPawn()->GetActorLocation(), CachedDestination);

				// 如果导航路径有效
				if (NavPath)
				{
					SplineComponent->ClearSplinePoints();
					for (const auto& PathPoint : NavPath->PathPoints)
					{
						// 向样条曲线添加点

						SplineComponent->AddSplinePoint(PathPoint, ESplineCoordinateSpace::World);
						// DrawDebugSphere(GetWorld(), PathPoint, 25, 10, FColor::Red, false, 5.f);
					}

					// 设置目标点为导航路径数组的最后一个点,防止无法到达某个位置或则跑出世界
					if (NavPath->PathPoints.Num() > 0)
					{
						CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					}
					BIsAutoWalking = true;
				}
			}
		}
		FollowCursorTime = 0;


		break;
	default:
		GetAbilitySystemComponent()->AbilityInputTagReleased(InputTag);

		break;
	}
}

void ABasePlayerController::AutoWalking()
{
	if (BIsAutoWalking && GetPawn())
	{
		// 从样条曲线中寻找离Pawn最近的点
		const auto Loc = SplineComponent->FindLocationClosestToWorldLocation(
			GetPawn()->GetActorLocation(), ESplineCoordinateSpace::World);
		// GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, *Loc.ToString());
		// 从样条曲线中寻找离Loc最近的方向
		const auto Dir = SplineComponent->FindDirectionClosestToWorldLocation(Loc, ESplineCoordinateSpace::World);
		GetPawn()->AddMovementInput(Dir);

		const auto DisToDes = (Loc - CachedDestination).Length();
		if (DisToDes <= StopDistance) { BIsAutoWalking = false; }
	}
}

void ABasePlayerController::ShiftPressed() { BIsShiftDown = true; }

void ABasePlayerController::ShiftReleased() { BIsShiftDown = false; }
