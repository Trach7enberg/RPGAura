// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250
#define COLLISION_PRESET_PROJECTILE FName("Projectile")					// 碰撞预设
#define ECC_PROJECTILE ECollisionChannel::ECC_GameTraceChannel1			// 默认情况下第一个通道是我们的自定义通道按顺序排列
#define TRACE_TYPE_QUERY_BEAM ETraceTypeQuery::TraceTypeQuery3			// 摄像机的射线检测的追踪通道
#define TRACE_TYPE_QUERY_CAMERA ETraceTypeQuery::TraceTypeQuery2		// 用于技能Beam射线检测的追踪通道
#define TRACE_TYPE_QUERY_VISIBILITY ETraceTypeQuery::TraceTypeQuery1	// 可见性的射线检测的追踪通道