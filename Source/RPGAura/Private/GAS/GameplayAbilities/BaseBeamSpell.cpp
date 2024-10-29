// Copyright GGBAO 


#include "GAS/GameplayAbilities/BaseBeamSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayCueFunctionLibrary.h"
#include "CoreTypes/RPGAuraGameplayTags.h"
#include "FunctionLibrary/RPGAuraBlueprintFunctionLibrary.h"
#include "Interfaces/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(UBaseBeamSpellLog, All, All);

void UBaseBeamSpell::TracingTarget(const FVector TracingStartPoint,
                                   TMap<AActor*, FGameplayCueParameters>& ActorToGcParam,
                                   const ETraceTypeQuery TraceChannel, const bool bDebug)
{
	if (!GetAvatarActorFromActorInfo()) { return; }

	const auto ComInt = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!ComInt) { return; }

	// 击中数组的结果顺序是按照射线击中的顺序来排列的
	const auto BlockingHit = UKismetSystemLibrary::SphereTraceMulti(this, TracingStartPoint, CursorImpactPoint, 10.f,
	                                                                TraceChannel, false,
	                                                                ActorsToIgnore,
	                                                                bDebug
		                                                                ? EDrawDebugTrace::ForDuration
		                                                                : EDrawDebugTrace::None,
	                                                                HitsResults, true);
	if (BlockingHit && HitsResults.Num())
	{
		// 获取第一个击中的Actor
		CursorImpactPoint = HitsResults.Last().ImpactPoint;
		FirstTraceDetectedActor = HitsResults[0].GetActor();
		BeamEndLoc = CursorImpactPoint;
		// 如果第一个被击中的Actor实现了战斗接口 ,则CueTarget是这个Actor
		CueTargetActor = FirstTraceDetectedActor;

		// 从第一个Actor开始,按照给定半径检测附近实现了combat接口并且存活的角色
		OverlapActors.Reset();
		URPGAuraBlueprintFunctionLibrary::FindLivePlayersWithinRadius(GetAvatarActorFromActorInfo(), OverlapActors,
		                                                              TArray<AActor*>{}, 650,
		                                                              BeamEndLoc, true,
		                                                              FRPGAuraGameplayTags::Get().Player,
		                                                              ChainReactionNum);

		if (bDebug)
		{
			UKismetSystemLibrary::DrawDebugSphere(this, FirstTraceDetectedActor->GetActorLocation(), 650.f, 16,
			                                      FLinearColor::White, 6.f);
			GEngine->AddOnScreenDebugMessage(1, 10, FColor::Green,
			                                 FString::Printf(
				                                 TEXT("[Limited -> %d]OverlapActors: %d"), ChainReactionNum,
				                                 OverlapActors.Num()));
		}

		if (!CanApplyGE() || !OverlapActors.Num())
		{
			// 否则CueTarget是我们的化身
			CueTargetActor = GetAvatarActorFromActorInfo();
			return;
		}
		const auto CombInterF = Cast<ICombatInterface>(GetStartCueTarget());
		if (!CombInterF->GetPreOnDeathDelegate().IsAlreadyBound(this, &UBaseBeamSpell::StartTargetPreOnDeath))
		{
			CombInterF->GetPreOnDeathDelegate().AddDynamic(this, &UBaseBeamSpell::StartTargetPreOnDeath);
		}

		// 设置被射线击中的起始角色进入Shock状态
		CombInterF->SetInShockHitState(true);
		if (const auto StartSufferAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetStartCueTarget()))
		{
			// 给被射线击中的第一个角色启用被电击的动画
			StartSufferAsc->TryActivateAbilitiesByTag(FGameplayTagContainer{
				FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact_InShock
			});
		}

		// 按照离当前玩家最近的位置开始进行排序 (这样MST最终的射线结果就会是从距离最短到最大依次有序)
		OverlapActors.Sort([this](const AActor& A, const AActor& B)
		{
			return (A.GetActorLocation() - BeamEndLoc).Length() < (B.GetActorLocation() - BeamEndLoc).Length();
		});

		// 从射线检测到的第一个角色开始,依次离当前角色最近的边集合
		// 注意:CloseEdges的Index代表OverlapActors,而对应的Index中的结构代表边,并且CloseEdges[StartPos]出发结点,权值无穷大
		// 例如:CloseEdges[0] = FCloseEdge{1,457},意思是从OverlapActors[0]到OverlapActors[1]的最近的边
		TArray<FCloseEdge> CloseEdges{};
		const auto StartPos = 1;
		FindBeamChain(CloseEdges, StartPos, bDebug);

		for (int i = 0; i < CloseEdges.Num(); ++i)
		{
			if (i == (StartPos - 1)) { continue; }
			ActorToGcParam.Add(OverlapActors[i],
			                   MakeBeamGameplayCueParameters(OverlapActors[i],
			                                                 OverlapActors[i]->GetRootComponent(),
			                                                 OverlapActors[CloseEdges[i].Adjvex]->
			                                                 GetActorLocation()));
		}
	}
}

FGameplayCueParameters UBaseBeamSpell::MakeBeamGameplayCueParameters(AActor* Source,
                                                                     USceneComponent* InTargetComp, const FVector& Loc)
{
	FGameplayCueParameters Parameters{};
	Parameters.SourceObject = Source;
	Parameters.TargetAttachComponent = InTargetComp;
	Parameters.Location = Loc;

	return Parameters;
}

AActor* UBaseBeamSpell::GetStartCueTarget() { return CueTargetActor.Get(); }


void UBaseBeamSpell::FindBeamChain(TArray<FCloseEdge>& CloseEdges,
                                   const int StartPos, const bool bDebug) const
{
	if (OverlapActors.Num() < 2) { return; }
	if (StartPos == 0) { return; }
	// 射线链的起点,即从OverlapActors中的哪一个开始进行链式反应
	const int Start = StartPos - 1;

	// 顶点数
	const int32 Vertex = GetValidChainNum();

	constexpr int32 Max = TNumericLimits<int32>::Max();

	// 矩阵图
	TArray<TArray<int32>> Graph{};
	// 标记已加入MST的数组
	TArray<bool> AlreadyAddToMst{};
	CloseEdges.Init(FCloseEdge{}, Vertex);
	AlreadyAddToMst.Init(false, Vertex);

	// 初始化图(顶点矩阵) 
	for (int i = 0; i < Vertex; ++i)
	{
		if (bDebug) { UE_LOG(UBaseBeamSpellLog, Error, TEXT("[结点%-2d:%23s]"), i+1, *OverlapActors[i]->GetName()); }
		TArray<int32> Elem{};
		for (int j = 0; j < Vertex; ++j)
		{
			// if (i == j) { continue; }
			auto Distance = (OverlapActors[j]->GetActorLocation() - OverlapActors[i]->
				GetActorLocation()).Length();
			Distance = (FMath::IsNearlyZero(Distance)) ? Max : Distance;
			const auto f = FString::Printf(
				TEXT("结点:%-2d -> |结点%2d : [%-23s]| -> 权重%-6.1f"), i + 1, j + 1, *OverlapActors[j]->GetName(),
				Distance);
			if (bDebug) { UE_LOG(UBaseBeamSpellLog, Warning, TEXT("%s"), *f); }
			Elem.Add(Distance);
		}
		Graph.Add(Elem);
	}

	// 初始化最近边数组
	for (int i = 0; i < Vertex; ++i)
	{
		CloseEdges[i].Adjvex = 0;
		// 设置从Start点开始,依次到其它点的初始权值
		CloseEdges[i].LowCost = Graph[i][Start];
	}
	// 标记起点已经加入MST
	AlreadyAddToMst[Start] = true;

	// Prim算法实现MST
	for (int i = 0; i < Vertex - 1; ++i)
	{
		int32 MinDistance = Max;
		int LowCostIndex = -1;
		for (int j = 0; j < Vertex; ++j)
		{
			if (AlreadyAddToMst[j]) { continue; }

			if (CloseEdges[j].LowCost < MinDistance)
			{
				MinDistance = CloseEdges[j].LowCost;
				LowCostIndex = j;
			}
		}
		if (LowCostIndex == -1) { break; }

		// 加入MST
		AlreadyAddToMst[LowCostIndex] = true;

		// 因为新加入了新边,所以得更新其它点到这个边的最近的位置
		for (int k = 0; k < Vertex; ++k)
		{
			if (AlreadyAddToMst[k]) { continue; }
			if (Graph[LowCostIndex][k] < CloseEdges[k].LowCost)
			{
				CloseEdges[k].Adjvex = LowCostIndex;
				CloseEdges[k].LowCost = Graph[LowCostIndex][k];
			}
		}
	}

	if (bDebug)
	{
		// 绘制Debug箭头
		for (int i = 0; i < Vertex; ++i)
		{
			if (i == Start) { continue; }
			const auto LineStart = OverlapActors[i]->GetActorLocation();
			auto LineEnd = OverlapActors[CloseEdges[i].Adjvex]->GetActorLocation();
			LineEnd.RotateAngleAxis(FMath::RandRange(-80, 80), FVector::RightVector);
			UE_LOG(LogTemp, Warning, TEXT("%f"), CloseEdges[i].LowCost);
			UKismetSystemLibrary::DrawDebugArrow(this, LineStart, LineEnd
			                                     , 1200,
			                                     FLinearColor::Green, 5, 4);
			UKismetSystemLibrary::DrawDebugString(
				this, (LineEnd - LineStart).GetSafeNormal() * (CloseEdges[i].LowCost / 3.f),
				FString::Printf(TEXT("[%d]%.1f"), i, CloseEdges[i].LowCost),
				OverlapActors[i], FLinearColor::Green, 5);
		}
	}
}

bool UBaseBeamSpell::CanEnableBeamChainReaction()
{
	if (!CanApplyGE() || !ActorToGamePlayCueParam.Num()) { return false; }
	return true;
}

int32 UBaseBeamSpell::GetValidChainNum() const { return FMath::Min(OverlapActors.Num(), ChainReactionNum); }

void UBaseBeamSpell::StartBeamChainReactionCue(TMap<AActor*, FGameplayCueParameters>& ActorToGcParam,
                                               const FGameplayTag GameplayCueTag)
{
	if (!CanEnableBeamChainReaction()) { return; }
	for (auto& Pair : ActorToGcParam)
	{
		UGameplayCueFunctionLibrary::AddGameplayCueOnActor(Pair.Key, GameplayCueTag, Pair.Value);
		if (const auto SufferAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pair.Key))
		{
			// 启用受害者被电击的动画
			SufferAsc->TryActivateAbilitiesByTag(FGameplayTagContainer{
				FRPGAuraGameplayTags::Get().Abilities_Effects_HitReact_InShock
			});
		}

		const auto ComInt = Cast<ICombatInterface>(Pair.Key);
		if (!ComInt) { continue; }
		// TODO 绑定目标死亡回调 
		if (ComInt->GetPreOnDeathDelegate().IsAlreadyBound(this, &UBaseBeamSpell::ChainTargetPreOnDeath)) { continue; }

		ComInt->GetPreOnDeathDelegate().AddDynamic(this, &UBaseBeamSpell::ChainTargetPreOnDeath);
		ComInt->SetInShockHitState(true);
	}
}

void UBaseBeamSpell::RemoveBeamChainReactionCue(TMap<AActor*, FGameplayCueParameters>& ActorToGcParam,
                                                const FGameplayTag GameplayCueTag)
{
	for (auto& Pair : ActorToGcParam)
	{
		UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(Pair.Key, GameplayCueTag, Pair.Value);
		const auto ComInt = Cast<ICombatInterface>(Pair.Key);
		if (!ComInt) { continue; }
		ComInt->SetInShockHitState(false);
	}

	ActorToGcParam.Reset();
}

void UBaseBeamSpell::RemoveAllBeamCue(const FGameplayTag StartCueTag, const FGameplayTag GameplayCueTag,
                                      const bool bRemoveStartCue)
{
	if (bRemoveStartCue)
	{
		UGameplayCueFunctionLibrary::RemoveGameplayCueOnActor(GetStartCueTarget(), StartCueTag,
		                                                      StartBeamGameplayCueParam);
	}

	if (const auto CombatI = Cast<ICombatInterface>(GetStartCueTarget())) { CombatI->SetInShockHitState(false); }
	RemoveBeamChainReactionCue(ActorToGamePlayCueParam, GameplayCueTag);
	bIsDeBuffSet = false;
}

bool UBaseBeamSpell::CanApplyGE() const
{
	// TODO 有很多冗余调用,是否Temp变量? 
	if (Cast<ICombatInterface>(FirstTraceDetectedActor)) { return true; }

	return false;
}

void UBaseBeamSpell::ApplyBeamGE()
{
	if (!CanApplyGE() || !OverlapActors.Num()) { return; }
	const auto LocalTmpDeBuffChance = DeBuffChance;
	DeBuffChance = (bIsDeBuffSet) ? 0.f : DeBuffChance;

	const auto LocalNum = GetValidChainNum();
	for (int i = 0; i < LocalNum; ++i) { CauseDamage(OverlapActors[i]); }

	// 恢复原值 (因为CauseDamage时候会传递DeBuffChance)
	// TODO CauseDamage 是否需要新增 参数进行控制,而不是用一个LocalTmpDeBuffChance 
	DeBuffChance = LocalTmpDeBuffChance;
	bIsDeBuffSet = true;
}
