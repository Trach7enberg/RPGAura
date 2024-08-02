// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class USplineComponent;
class UBaseAbilitySystemComponent;
struct FGameplayTag;
class URPGAuraInputConfig;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

/**
 * 基础玩家控制类
 */
UCLASS()
class RPGAURA_API ABasePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABasePlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

private:
    // 当前鼠标击中的Actor
    UPROPERTY()
    TObjectPtr<AActor> CurrentActor;

    // 鼠标击中Actor时的HitResult
    UPROPERTY()
    FHitResult CurseHitResult;
    /// 输入上下文
    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UInputMappingContext> InputContext;

    //IA_Move
    UPROPERTY(EditAnywhere, Category="Input")
    TObjectPtr<UInputAction> MoveAction;

    void Move(const FInputActionValue& InputActionValue);

    UPROPERTY(EditDefaultsOnly, Category="Input")
    TObjectPtr<URPGAuraInputConfig> InputConfig;

    UPROPERTY()
    TObjectPtr<UBaseAbilitySystemComponent> AbilitySystemComponent;

    // 样条线组件,用于平滑路径
    UPROPERTY()
    TObjectPtr<USplineComponent> SplineComponent;

    // 存储目标点
    FVector CachedDestination;

    // 人物跟随鼠标的时间
    float FollowCursorTime;

    // 短按阈值,超过这个值就是短按,不是点击,以秒为单位 TODO 应该在增强输入IA或者映射里设置触发器
    float ShortPressThreshold;

    // 是否自动行走
    bool BIsAutoRunning;

    // 鼠标是否在瞄准目标
    bool BIsTargeting;

    // 距离目标多近就停下来
    UPROPERTY(EditDefaultsOnly, Category="Move")
    float StopDistance;

    UBaseAbilitySystemComponent* GetAbilitySystemComponent();
    void AbilityInputTagPressed(FGameplayTag InputTag);
    void AbilityInputTagReleased(FGameplayTag InputTag);
    void AbilityInputTagHeld(FGameplayTag InputTag);
};