// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250
#define COLLISION_PRESET_PROJECTILE FName("Projectile")	// 碰撞预设
#define ECC_PROJECTILE ECollisionChannel::ECC_GameTraceChannel1 // 默认情况下第一个通道是我们的自定义通道按顺序排列