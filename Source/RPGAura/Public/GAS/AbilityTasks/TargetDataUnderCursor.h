// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderCursor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignatue, const FVector&, Data);

/**
 * 这个能力任务将负责获取与光标下的击中结果相关的数据
 */
UCLASS()
class RPGAURA_API UTargetDataUnderCursor : public UAbilityTask
{
    GENERATED_BODY()

public:

    // 输出引脚
    UPROPERTY(BlueprintAssignable)
    FMouseTargetDataSignatue OnGetTargetDataUnderCursor;
    
    /// 获取鼠标击中的HitResult
    /// 该函数实际上只是一个工厂,创建这个任务的实例并返回,因此使用宏参数BlueprintInternalUseOnly = "TRUE"
    /// 同时也不需要手动传入OwningAbility,因此需要HidePin = "OwningAbility",并且把这个参数设置为this,所以再引入宏标签DefaultToSelf = "OwningAbility"
    /// @param OwningAbility 当前的能力,其实就是This(Self)
    /// @return 能力任务实例
    UFUNCTION(BlueprintCallable,
        Category="Ability|Tasks",
        meta = (DisplayName="GetTargetDataUnderCursor",
            HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UTargetDataUnderCursor* CreateTargetDataUnderCursorProxy(UGameplayAbility* OwningAbility);

protected:
    virtual void Activate() override;
};