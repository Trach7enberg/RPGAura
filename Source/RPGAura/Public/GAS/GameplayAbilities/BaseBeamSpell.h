// Copyright GGBAO 

#pragma once

#include "CoreMinimal.h"
#include "GAS/GameplayAbilities/BaseDamageAbility.h"
#include "BaseBeamSpell.generated.h"

USTRUCT()
struct FCloseEdge
{
	GENERATED_BODY()

	FCloseEdge() {}

	/// 在overlapping中的Actor下标 
	UPROPERTY()
	int32 Adjvex = 0;

	/// 权值
	UPROPERTY()
	float LowCost = 0;
};

/**
 * 基础射线法术类 
 */
UCLASS()
class RPGAURA_API UBaseBeamSpell : public UBaseDamageAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector GetCursorImpactPoint() const { return CursorImpactPoint; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCursorImpactPoint(const FVector& NewCursorImpactPoint)
	{
		this->CursorImpactPoint = NewCursorImpactPoint;
	}

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetUnderCursorHitActor(AActor* NewUnderCursorHitActor)
	{
		this->UnderCursorHitActor = NewUnderCursorHitActor;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FVector GetBeamEndLoc() { return this->BeamEndLoc; }

	/// 获取第一次启动射线所需要的CueTarget
	/// @return 
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AActor* GetStartCueTarget();

	/// 
	/// @param Source
	/// @param InTargetComp
	/// @param Loc
	/// @return 
	UFUNCTION(BlueprintCallable)
	FGameplayCueParameters MakeBeamGameplayCueParameters(AActor* Source,
	                                                     USceneComponent* InTargetComp,
	                                                     UPARAM(REF)const FVector& Loc);

	/// 多重射线检测 TODO 技能升级后才允许穿透,默认等级不带穿透效果
	/// @param TracingStartPoint
	/// @param ActorToGcParam 
	/// @param TraceChannel
	/// @param bDebug 
	UFUNCTION(BlueprintCallable)
	void TracingTarget(const FVector TracingStartPoint,
	                   UPARAM(REF) TMap<AActor*, FGameplayCueParameters>& ActorToGcParam,
	                   ETraceTypeQuery TraceChannel, bool bDebug = true);

	/// 启动射线的链式反应
	/// @param ActorToGcParam 
	/// @param GameplayCueTag 
	UFUNCTION(BlueprintCallable)
	void StartBeamChainReactionCue(UPARAM(REF)TMap<AActor*, FGameplayCueParameters>& ActorToGcParam,
	                               const FGameplayTag GameplayCueTag);

	/// 移除射线的链式反应
	/// @param ActorToGcParam 
	/// @param GameplayCueTag
	UFUNCTION(BlueprintCallable)
	void RemoveBeamChainReactionCue(UPARAM(REF)TMap<AActor*, FGameplayCueParameters>& ActorToGcParam,
	                                const FGameplayTag GameplayCueTag);

	/// 对射线目标应用GE
	UFUNCTION(BlueprintCallable)
	void ApplyBeamGE();

	/// 移除所有Cue
	/// @param StartCueTag
	/// @param GameplayCueTag
	/// @param bRemoveStartCue 
	UFUNCTION(BlueprintCallable)
	void RemoveAllBeamCue(FGameplayTag StartCueTag, FGameplayTag GameplayCueTag, bool bRemoveStartCue);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Prop|Other")
	FTimerHandle TimerHandle{};

	/// 当前射线是否是穿透
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Beam")
	bool BIsPenetration = false;

	/// 是否启用射线穿透
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Beam")
	bool BIsChainReaction = true;

	// 射线链数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Beam", meta=(EditCondition="BIsChainReaction"))
	int ChainReactionNum = 5;

	/// 鼠标检测击中的目标
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Prop|HitActor")
	TObjectPtr<AActor> UnderCursorHitActor = nullptr;

	/// 释放射线时的射线起点
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Prop|Loc")
	FVector WeaponTipLoc = FVector::ZeroVector;

	/// 当前射线的终点
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Prop|Loc")
	FVector BeamEndLoc = FVector::ZeroVector;

	// 每一个(射线链中的)Actor对应的GameplayCueParam
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Prop|GCN")
	TMap<AActor*, FGameplayCueParameters> ActorToGamePlayCueParam{};

	/// 第一次启动射线的检测结果(不是射线链的HitResult)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Prop|HitResult")
	TArray<FHitResult> HitsResults{};

	/// 第一次启动射线Cue所需要的Cue参数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Prop|GCN")
	FGameplayCueParameters StartBeamGameplayCueParam{};

	/// 射线目标击中死亡的回调函数
	/// @param ChainTarget
	UFUNCTION(BlueprintImplementableEvent)
	void ChainTargetPreOnDeath(AActor* ChainTarget);

	/// 触发射线链的第一个角色死亡时的回调函数
	UFUNCTION(BlueprintImplementableEvent)
	void StartTargetPreOnDeath(AActor* StartTarget);

	virtual int32 GetValidAbilityCount(const int32 AbilityLevel) override;

private:
	/// 每次启动射线都只会有几率触发一次DeBuff效果,而不是射线期间持续判断触发
	UPROPERTY()
	bool bIsDeBuffSet = false;

	/// 激活链式反应前所检测到Actors
	UPROPERTY()
	TArray<AActor*> OverlapActors{};

	/// 第一次启动射线(GCN)所需要的CueTarget,不是启动射线链GCN的CueTarget
	UPROPERTY()
	TObjectPtr<AActor> CueTargetActor = nullptr;

	/// 第一次启动射线GCN,由光线检测击中到的(第一个)目标
	UPROPERTY()
	TObjectPtr<AActor> FirstTraceDetectedActor = nullptr;


	/// 鼠标击中的冲击点向量(启动射线技能时的点击)
	UPROPERTY()
	FVector CursorImpactPoint = FVector::ZeroVector;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UFUNCTION(BlueprintCallable)
	bool CanApplyGE() const;

	/// 能否启用射线链式反应
	/// @return 
	bool CanEnableBeamChainReaction();

	int32 GetValidChainNum() const;

	/// 使用Prim算法计算最小生成树,查找由射线造成的射线链
	/// @param CloseEdges 找到的射线链
	/// @param StartPos 射线链的开始位置,即从OverlappingActors中的某个Actor开始
	/// @param bDebug
	void FindBeamChain(TArray<FCloseEdge>& CloseEdges, int StartPos,
	                   bool bDebug = false) const;
};
