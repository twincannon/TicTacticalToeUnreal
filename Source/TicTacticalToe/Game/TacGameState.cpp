// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacGameState.h"
#include "TicTacticalToe/Game/TacBoardTile.h"


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
			Board = GetWorld()->SpawnActor<ATacBoard>(BoardClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (Board)
			{
				Board->OnBoardGameOver.AddDynamic(this, &ATacGameState::OnBoardGameOver);
			}
		}
		else
		{
			// Failed to make game board? Return to mainmenu
			UE_LOG(LogTemp, Warning, TEXT("Failed to create ATacBoard"));
			ChangeState(EGameState::MAINMENU);
		}
	}
	
	OnStateChanged.Broadcast(NewState);
}

void ATacGameState::PassTurn()
{
	if (CurrentState != EGameState::GAME_TICTACTOE)
	{
		return;
	}

	bIsPlayersTurn = !bIsPlayersTurn;

	if (!bIsPlayersTurn)
	{
		GetWorld()->GetTimerManager().SetTimer(OpponentTurnTimer, this, &ATacGameState::OpponentTurn, 0.5f, false);
	}
}

void ATacGameState::OpponentTurn()
{
	// Get game board, get random empty tile (for now), change it to X and then pass the turn
	if (IsValid(Board))
	{
		ATacBoardTile* const tile = Board->GetRandomEmptyTile();
		if (IsValid(tile))
		{
			tile->SetTileType(ETileType::EX);
		}
	}

	PassTurn();
}

void ATacGameState::OnBoardGameOver(EPlayerType Winner)
{
	// Current hex changes ownership to winner
}