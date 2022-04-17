// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterAction : uint8
{
	Idle = 0,
	Attack,
	DashAttack,
	Guard,
	Evade,
	Hitstun,
	Knockback
};