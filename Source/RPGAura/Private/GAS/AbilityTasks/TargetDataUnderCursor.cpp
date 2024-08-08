// Copyright GGBAO 


#include "GAS/AbilityTasks/TargetDataUnderCursor.h"

#include "AbilitySystemComponent.h"

DEFINE_LOG_CATEGORY_STATIC(UTargetDataUnderCursorLog, All, All);

UTargetDataUnderCursor* UTargetDataUnderCursor::CreateTargetDataUnderCursorProxy(UGameplayAbility* OwningAbility)
{
	const auto Result = NewAbilityTask<UTargetDataUnderCursor>(OwningAbility);
	return Result;
}

void UTargetDataUnderCursor::Activate()
{
	// 当前不在服务器控制的话就发送(HitResult)数据给服务器
	if (Ability && Ability->GetActorInfo().IsLocallyControlled()) { SendTargetDataToServer(); }
	else
	{
		// 当前已经在服务器,只需要监听发送过来的数据,当服务接受数据会广播一个委托,因此我们获取这个委托即可
		AbilitySystemComponent.Get()->
		                       AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).
		                       AddUObject(this, &UTargetDataUnderCursor::OnTargetDataReplicatedCallBack);

		// 当服务器广播委托先于我们还没有绑定那个接受数据的委托时,导致我们没有执行回调函数,就用下面这个函数,返回值为true说明数据已经成功到达,并且我们的委托已经被绑定
		const bool BIsCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(
			GetAbilitySpecHandle(),
			GetActivationPredictionKey());

		// 委托成功绑定但是客户端的数据还没送过来,我们需要等待那个数据
		if (!BIsCalledDelegate) { SetWaitingOnRemotePlayerData(); }
	}
}

void UTargetDataUnderCursor::SendTargetDataToServer()
{
	const auto ActorInfo = Ability->GetCurrentActorInfo();
	if (!ActorInfo) { return; }

	if (!ActorInfo->PlayerController.Get()) { return; }

	if (!ActorInfo->PlayerController.Get()) { return; }


	// 创建一个范围的预测窗口,说明当前这个函数(SendTargetDataToServer)的范围内所有内容都应该被预测
	FScopedPredictionWindow ScopedPredictionWindow(ActorInfo->AbilitySystemComponent.Get());


	const auto Pc = ActorInfo->PlayerController.Get();
	FHitResult HitResult;
	Pc->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	// 创建一个要发送给服务器FHitResult数据的结构体
	// 注意,如果开始游戏报错显示什么ScriptStructCache的话你需要在未使用targetData这个结构体之前用UAbilitySystemGlobals::Get().InitGlobalData()这个函数初始化一下
	const auto HitData = new FGameplayAbilityTargetData_SingleTargetHit();
	HitData->HitResult = HitResult;

	// 包裹FHitResult数据的一个Handle
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(HitData);

	// 发送FHitResult数据到服务器(该函数内部有一个委托会进行广播)
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey(),
	                                                      DataHandle, FGameplayTag(),
	                                                      AbilitySystemComponent->ScopedPredictionKey);

	// ShouldBroadcastAbilityTaskDelegates 函数的作用主要是决定是否应该广播能力任务的委托,
	// 只有能力还在active的时候才会广播,能力没激活就不广播了
	// 在某些情况下，如果任务不需要广播事件,可判断 false 来减少不必要的性能开销
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		// 将委托的参数改为DataHandle类
		// 发送FHitResult的Handle到客户端,这样我们除了可以拿到HitResult之外,还可以获得其它有用的信息
		OnGetTargetDataUnderCursor.Broadcast(DataHandle);
	}
}

void UTargetDataUnderCursor::OnTargetDataReplicatedCallBack(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                            FGameplayTag ActivationTag)
{
	// 数据只会从服务器复制到客户端,但我们也可以将复制的数据通过RPC从客户端发送到服务器,所以有时候也可以叫"Replicated Data"
	// 接收数据的时候还要通知能力系统组件,因为这个数据存储在一个特定的结构里,因此我们需要通知Asc你不需要存储它，也不需要再缓存它
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates()) { OnGetTargetDataUnderCursor.Broadcast(DataHandle); }
}
