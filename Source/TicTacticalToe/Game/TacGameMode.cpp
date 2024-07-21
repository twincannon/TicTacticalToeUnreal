// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacGameMode.h"
#include "TicTacticalToe/Game/TacGameState.h"
#include "TicTacticalToe/Game/TacPawn.h"

ATacGameMode::ATacGameMode()
{
	GameStateClass = ATacGameState::StaticClass();
	DefaultPawnClass = ATacPawn::StaticClass();
}

void ATacGameMode::StartPlay()
{
	Super::StartPlay();

	StartMatch();
}
