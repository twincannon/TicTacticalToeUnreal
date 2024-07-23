// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define COLLISION_CLICK ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_HEX ECollisionChannel::ECC_GameTraceChannel2

UENUM(BlueprintType)
enum EGameState {
	MAINMENU = 0,
	GAME_OVERVIEW,
	GAME_TICTACTOE,
	GAMEOVER
};

UENUM(BlueprintType)
enum ETileType {
	EMPTY = 0,
	OH,
	EX,
	RADIOACTIVE
};

UENUM(BlueprintType)
enum EPlayerType {
	PLAYER = 0,
	OPPONENT,
	NEUTRAL
};