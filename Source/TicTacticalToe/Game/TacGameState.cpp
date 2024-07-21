// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacGameState.h"
//#include "Kismet/GameplayStatics.h"


void ATacGameState::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(EGameState::MAINMENU);
}

void ATacGameState::ChangeState(EGameState NewState)
{
	CurrentState = NewState;

	if (NewState == EGameState::GAME_TICTACTOE)
	{
		if (BoardClass)
		{
			ATacBoard* newBoard = GetWorld()->SpawnActor<ATacBoard>(BoardClass, FVector::ZeroVector, FRotator::ZeroRotator);
		}
		else
		{
			// Failed to make game board? Return to mainmenu
			ChangeState(EGameState::MAINMENU);
		}
	}
	
	OnStateChanged.Broadcast(NewState);
}

void ATacGameState::PassTurn()
{
	bIsPlayersTurn = !bIsPlayersTurn;

	if (!bIsPlayersTurn)
	{
		GetWorld()->GetTimerManager().SetTimer(OpponentTurnTimer, this, &ATacGameState::OpponentTurn, 0.5f, false);
	}
}

void ATacGameState::OpponentTurn()
{
	// Get game board, get random empty tile, change it to X and passturn

	//UGameplayStatics::GetAllActorsOfClass(this, ATacBoard::StaticClass(), outBoards);
}
