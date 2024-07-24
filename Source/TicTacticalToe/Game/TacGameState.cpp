// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacGameState.h"
#include "TicTacticalToe/Game/TacBoardTile.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


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

		const float nextTurnTimer = 1.f;

		if (Board)
		{
			DestroyBoard();
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
	else if (NewState == EGameState::GAMEOVER)
	{
		if (Board)
		{
			Board->Destroy();
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

	if (GetWorldTimerManager().IsTimerActive(BoardGameOverTimer))
	{
		// We triggered game over, don't queue up another turn
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
	// Opponent is smarter than neutral and tries to close off and secure wins
	if (IsValid(Board) && CurrentState == EGameState::GAME_TICTACTOE)
	{
		ATacBoardTile* randomTile = nullptr;
		TArray<ATacBoardTile*> winTiles = Board->GetPotentialWinningTiles();
		
		if (winTiles.Num() >= 1)
		{
			// Actually picking the first found tile instead of a random one is smarter, amusingly (since it means that it will prioritize tile type OH/offense)
			randomTile = winTiles[0]; //winTiles[FMath::RandRange(0, winTiles.Num() - 1)];
		}
		else
		{
			randomTile = Board->GetRandomEmptyTile();
		}

		if (IsValid(randomTile))
		{
			if (OffensiveTeam == EPlayerType::OPPONENT)
			{
				randomTile->SetTileType(ETileType::OH);
			}
			else
			{
				randomTile->SetTileType(ETileType::EX);
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

	if (SelectedHex)
	{
		// If player or opponent lost to neutral it needs to stay neutral, NOT turn to the opposing side
		if (DidOffenseWin)
		{
			if (SelectedHex->HexHQType == EPlayerType::OPPONENT)
			{
				WinningTeam = EPlayerType::PLAYER;
			}
			else if (SelectedHex->HexHQType == EPlayerType::PLAYER)
			{
				WinningTeam = EPlayerType::OPPONENT;
			}

			SelectedHex->SetOwnership(OffensiveTeam);

			if (OffensiveTeam == EPlayerType::PLAYER)
			{
				// Give player an item for taking a hex
				OnPlayerWonHex();
			}

			if (WinningTeam != EPlayerType::NONE)
			{
				FTimerDelegate timerDel = FTimerDelegate::CreateUObject(this, &ATacGameState::ChangeState, EGameState::GAMEOVER);
				GetWorldTimerManager().SetTimer(BoardGameOverTimer, timerDel, 3.f, false);
			}
		}
	}

	if (IsBoardGameOver() == false)
	{
		FTimerDelegate timerDel = FTimerDelegate::CreateUObject(this, &ATacGameState::ChangeState, EGameState::GAME_OVERVIEW);
		GetWorldTimerManager().SetTimer(BoardGameOverTimer, timerDel, 3.f, false);
	}

	GetWorldTimerManager().ClearTimer(OpponentTicTacToeTurnTimer);
}

bool ATacGameState::IsBoardGameOver()
{
	return GetWorldTimerManager().IsTimerActive(BoardGameOverTimer);
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
		SelectedHex = hexGrid->GetRandomOpponentCapturableHex(); // This highlights the hexes the opponent can choose from

		if (SelectedHex)
		{
			FTimerHandle handle;
			GetWorldTimerManager().SetTimer(handle, this, &ATacGameState::OpponentConfirmHex, 3.f, false);
		}
		else
		{
			// If opponent didn't choose a hex (because of radiation or such), pass the turn to the player
			ChangeState(EGameState::GAME_OVERVIEW);
		}
	}
}

void ATacGameState::OpponentConfirmHex()
{
	if (SelectedHex)
	{
		SelectedHex->OnHexChosen();

		OffensiveTeam = EPlayerType::OPPONENT;
		DefensiveTeam = SelectedHex->GetOwningPlayer();

		if (ATacHexGrid* hexGrid = GetHexGrid())
		{
			hexGrid->ClearCapturableHexes();
		}

		FTimerHandle handle;
		FTimerDelegate timerDel = FTimerDelegate::CreateUObject(this, &ATacGameState::ChangeState, EGameState::GAME_TICTACTOE);
		GetWorldTimerManager().SetTimer(handle, timerDel, 1.f, false);
	}
}

void ATacGameState::SetSelectingTileForItem(bool bIsSelecting)
{
	bIsSelectingTileForItem = bIsSelecting;

	if (APlayerController* pc = GetWorld()->GetFirstPlayerController())
	{
		pc->CurrentMouseCursor = bIsSelecting ? EMouseCursor::Crosshairs : EMouseCursor::Default;
		UWidgetBlueprintLibrary::SetFocusToGameViewport();
	}
}