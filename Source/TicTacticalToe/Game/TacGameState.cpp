// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacGameState.h"
#include "TicTacticalToe/Game/TacBoardTile.h"
#include "Kismet/GameplayStatics.h"


void ATacGameState::BeginPlay()
{
	Super::BeginPlay();

	ChangeState(EGameState::MAINMENU);
}

void ATacGameState::ChangeState(EGameState NewState)
{
	CurrentState = NewState;

	ATacHexGrid* hexGrid = GetHexGrid();
	if (hexGrid == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't find HexGrid"));
		return;
	}

	bIsPlayersTicTacToeTurn = false;
	bIsTicTacToeOffenseTurn = true;
	hexGrid->ClearCapturableHexes();
	GetWorldTimerManager().ClearTimer(OpponentTicTacToeTurnTimer);
	
	if (NewState == EGameState::GAME_TICTACTOE)
	{
		if (BoardClass)
		{
			Board = GetWorld()->SpawnActor<ATacBoard>(BoardClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (Board)
			{
				Board->OnBoardGameOver.AddDynamic(this, &ATacGameState::OnBoardGameOver);

				TicTacToeTeamTurn = OffensiveTeam;
				if (TicTacToeTeamTurn == EPlayerType::PLAYER)
				{
					bIsPlayersTicTacToeTurn = true;
				}
				else
				{
					GetWorldTimerManager().SetTimer(OpponentTicTacToeTurnTimer, this, &ATacGameState::OpponentTicTacToeTurn, TurnTimer, false);
				}
			}
		}
		else
		{
			// Failed to make game board? Return to mainmenu
			UE_LOG(LogTemp, Warning, TEXT("Failed to create ATacBoard"));
			ChangeState(EGameState::MAINMENU);
		}
	}
	else if (NewState == EGameState::GAME_OVERVIEW)
	{
		bIsPlayersHexTurn = !bIsPlayersHexTurn;

		const float destroyBoardTimer = 2.f;
		const float nextTurnTimer = destroyBoardTimer + 1.f;
		if (Board)
		{
			FTimerHandle handle;
			GetWorldTimerManager().SetTimer(handle, this, &ATacGameState::DestroyBoard, destroyBoardTimer, false);
		}

		if (bIsPlayersHexTurn)
		{
			FTimerHandle handle;
			GetWorldTimerManager().SetTimer(handle, this, &ATacGameState::SetCapturableHexesTimerDone, nextTurnTimer, false);
		}
		else
		{
			FTimerHandle handle;
			GetWorldTimerManager().SetTimer(handle, this, &ATacGameState::OpponentChooseHex, nextTurnTimer, false);
		}
	}
	
	OnStateChanged.Broadcast(NewState);
}

void ATacGameState::SetCapturableHexesTimerDone()
{
	if (ATacHexGrid* hexGrid = GetHexGrid())
	{
		hexGrid->SetCapturableHexes();
	}
}

void ATacGameState::DestroyBoard()
{
	if (Board)
	{
		Board->Destroy();
	}
}

void ATacGameState::PassTicTacToeTurn()
{
	if (CurrentState != EGameState::GAME_TICTACTOE)
	{
		return;
	}

	bIsPlayersTicTacToeTurn = false;
	bIsTicTacToeOffenseTurn = !bIsTicTacToeOffenseTurn;


	if (bIsTicTacToeOffenseTurn)
	{
		switch (OffensiveTeam)
		{ // Neutral can never be offense
		case EPlayerType::PLAYER:
			bIsPlayersTicTacToeTurn = true;
			break;
		case EPlayerType::OPPONENT:
			GetWorldTimerManager().SetTimer(OpponentTicTacToeTurnTimer, this, &ATacGameState::OpponentTicTacToeTurn, TurnTimer, false);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (DefensiveTeam)
		{
		case EPlayerType::PLAYER:
			bIsPlayersTicTacToeTurn = true;
			break;
		case EPlayerType::OPPONENT:
			GetWorldTimerManager().SetTimer(OpponentTicTacToeTurnTimer, this, &ATacGameState::OpponentTicTacToeTurn, TurnTimer, false);
			break;
		case EPlayerType::NEUTRAL:
			GetWorldTimerManager().SetTimer(OpponentTicTacToeTurnTimer, this, &ATacGameState::NeutralTicTacToeTurn, TurnTimer, false);
			break;
		default:
			break;
		}
	}
}

void ATacGameState::OpponentTicTacToeTurn()
{
	// As a first rudimentary AI type thing let's make it so the opponent always tries to close off player wins (if col/row has 2 player owned tiles and an empty, choose the empty)
	if (IsValid(Board) && CurrentState == EGameState::GAME_TICTACTOE)
	{
		ATacBoardTile* const tile = Board->GetRandomEmptyTile();
		if (IsValid(tile))
		{
			if (OffensiveTeam == EPlayerType::OPPONENT)
			{
				tile->SetTileType(ETileType::OH);
			}
			else
			{
				tile->SetTileType(ETileType::EX);
			}
		}
	}

	PassTicTacToeTurn();
}

void ATacGameState::NeutralTicTacToeTurn()
{
	// Neutral is dumb and just chooses random empty tiles
	if (IsValid(Board) && CurrentState == EGameState::GAME_TICTACTOE)
	{
		ATacBoardTile* const tile = Board->GetRandomEmptyTile();
		if (IsValid(tile))
		{
			// Neutral can only be defense
			tile->SetTileType(ETileType::EX);
		}
	}

	PassTicTacToeTurn();
}

void ATacGameState::OnBoardGameOver(bool DidOffenseWin)
{
	
	// Current hex changes ownership to winner

	if (ATacHex* hex = SelectedHex.Get())
	{
		// If player or opponent lost to neutral it needs to stay neutral, NOT turn to the opposing side
		// This actually brings into question the idea of there being a neutral force at all, since that means AI plays vs AI?
		if (DidOffenseWin)
		{
			hex->SetOwnership(OffensiveTeam);
		}
	}

	ChangeState(EGameState::GAME_OVERVIEW);
}

ATacHexGrid* ATacGameState::GetHexGrid()
{
	if (IsValid(HexGrid))
	{
		return HexGrid;
	}
	else
	{
		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATacHexGrid::StaticClass(), outActors);
		if (outActors.Num() > 0)
		{
			HexGrid = Cast<ATacHexGrid>(outActors[0]);
		}
	}

	return HexGrid;
}

void ATacGameState::OpponentChooseHex()
{
	if (ATacHexGrid* hexGrid = GetHexGrid())
	{
		SelectedHex = hexGrid->GetRandomOpponentCapturableHex();
		if (SelectedHex)
		{
			OffensiveTeam = EPlayerType::OPPONENT;
			DefensiveTeam = SelectedHex->GetOwningPlayer();

			hexGrid->ClearCapturableHexes();

			FTimerHandle handle;
			FTimerDelegate timerDel = FTimerDelegate::CreateUObject(this, &ATacGameState::ChangeState, EGameState::GAME_TICTACTOE);

			SelectedHex->OnHexChosen();
			
			GetWorldTimerManager().SetTimer(handle, timerDel, 2.f, false);
		}
	}
}