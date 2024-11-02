// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250
#define COLLISION_PRESET_PROJECTILE FName("Projectile")						// 碰撞预设 射弹
#define COLLISION_PRESET_DETECT_GROUND_POINTS FName("DetectGroundPoints")	// 碰撞预设 用于检测地面上的点

#define OBJECT_CHANNEL_MAGICDECAL ECollisionChannel::ECC_GameTraceChannel3	// 用于魔法贴花显示
#define OBJECT_CHANNEL_PROJECTILE ECollisionChannel::ECC_GameTraceChannel1	// 用于飞弹
