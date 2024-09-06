// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitForCoolDown.generated.h"

struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;
struct FGameplayTag;
class UAbilitySystemComponent;

// 用于广播冷却剩余时间的委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoolDownChangeSignature, float, TimeRemaining);

/**
 * 等待蓝图类中的CoolDown
 */
UCLASS(BlueprintType, meta=(ExposedAsyncProxy= "WaitForCoolDownAsyncTask")) // 暴露由静态函数WaitCoolDownChange创建的引用返回值给蓝图
class RPGAURA_API UWaitForCoolDown : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	// 委托作为蓝图结点的输出引脚
	UPROPERTY(BlueprintAssignable)
	FCoolDownChangeSignature CoolDownStart;

	// 委托作为蓝图结点的输出引脚
	UPROPERTY(BlueprintAssignable)
	FCoolDownChangeSignature CoolDownEnd;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"))
	static UWaitForCoolDown* WaitCoolDownChange(UAbilitySystemComponent* AbilitySystemComponent,
	                                            const FGameplayTag& InCoolDownTag);

	// 结束、摧毁当前结点任务
	UFUNCTION(BlueprintCallable)
	void EndTask();

private:
	// 暂存蓝图输入节点的AbilitySystemComponent变量
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> Asc;

	// 暂存蓝图输入节点的InCoolDownTag变量
	FGameplayTag CoolDownTag;

	
	FDelegateHandle OnActiveGameplayEffectDelegateHandle;

	/// 当Asc获得冷却标签时回调的函数
	/// @param InCooldownTag 
	/// @param NewCount 
	UFUNCTION()
	void OnCoolDownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);

	/// 当Asc每当添加基于持续时间的GE时的回调的函数
	/// @param AbilitySystemComponent 
	/// @param GeSpec 
	/// @param ActiveEffectHandle 
	UFUNCTION()
	void OnActiveGameplayEffectAdded(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GeSpec,
	                                 FActiveGameplayEffectHandle ActiveEffectHandle);
};
