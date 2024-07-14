// Copyright GGBAO 


#include "Controller/BasePlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"

DEFINE_LOG_CATEGORY_STATIC(MyABasePlayerController,All,All);

ABasePlayerController::ABasePlayerController()
{
	// 启用复制
	bReplicates = true; 
	bShowMouseCursor = true;

	// 设置鼠标的样式
	DefaultMouseCursor = EMouseCursor::Type::Default;
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(InputContext);

	// 获得增强输入的本地玩家子系统,通过这个根据添加我们的上下文映射
	const auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(SubSystem);

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
	if (!InputContext) { return; }

	// 输入组件转换为增强输入组件
	const auto Input = CastChecked<UEnhancedInputComponent>(InputComponent);

	Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasePlayerController::Move);
}

void ABasePlayerController::Move(const FInputActionValue &InputActionValue)
{
	const auto InputAxisValue = InputActionValue.Get<FVector2D>();
	if (GetPawn())
	{
		GetPawn()->AddMovementInput(GetControlRotation().Euler().ForwardVector, InputAxisValue.Y);
		GetPawn()->AddMovementInput(GetControlRotation().Euler().RightVector, InputAxisValue.X);
	}
}

