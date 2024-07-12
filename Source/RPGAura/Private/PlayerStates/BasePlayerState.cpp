// Copyright GGBAO 


#include "PlayerStates/BasePlayerState.h"

ABasePlayerState::ABasePlayerState()
{
	// 当玩家状态在服务器上发生变化时,服务器就会发送更新到所有客户端 这个值就是设置服务器发送更新到所有客户端的频率
	// 但是当玩家状态有GAS系统的组件时,这个值要设置高(慢)一点
	NetUpdateFrequency = 100.f;
}
