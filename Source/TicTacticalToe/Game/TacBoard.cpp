// Fill out your copyright notice in the Description page of Project Settings.

#include "TicTacticalToe/Game/TacBoard.h"
#include "TicTacticalToe/Game/TacGameState.h"
#include "TicTacticalToe/TicTacticalToe.h"

// Sets default values
ATacBoard::ATacBoard()
{
	BoardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BoardRoot"));
	RootComponent = BoardRoot;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATacBoard::BeginPlay()
{
	Super::BeginPlay();
	
	SetupTiles();
}

// Called every frame
void ATacBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATacBoard::SetupTiles()
{
	TileRows.SetNum(BoardSize.X);
	for (int32 i = 0; i < BoardSize.X; ++i)
	{
		for (int32 j = 0; j < BoardSize.Y; ++j)
		{
			if (TileClass)
			{
				UChildActorComponent* newTile = NewObject<UChildActorComponent>(this);
				newTile->SetChildActorClass(TileClass);
				newTile->SetupAttachment(BoardRoot);
				newTile->CreateChildActor();

				ATacBoardTile* tile = Cast<ATacBoardTile>(newTile->GetChildActor());
				if (IsValid(tile))
				{
					tile->SetOwner(this);
					tile->OnTileClicked.AddDynamic(this, &ATacBoard::TileClickedCallback);
					tile->OnTileTypeChanged.AddDynamic(this, &ATacBoard::TileChanged);
					tile->InitializeTile(FIntPoint(i, j));

					TileRows[i].TileCol.Add(tile);

					const float tileSpacing = tile->GetTileSpacing();
					const float tileOffset = -tileSpacing * (BoardSize.X / 2.f);
					newTile->SetRelativeLocation(FVector(tileOffset + tileSpacing * i, tileSpacing * j, 0.f));
				}
			}
		}
	}
}

void ATacBoard::TileClickedCallback(ATacBoardTile* const Tile)
{
	if (ATacGameState* gameState = Cast<ATacGameState>(GetWorld()->GetGameState()))
	{
		if (gameState->IsPlayersTicTacToeTurn() && gameState->CurrentState == EGameState::GAME_TICTACTOE)
		{
			if (Tile->TileType == ETileType::EMPTY)
			{
				Tile->SetTileType(gameState->OffensiveTeam == EPlayerType::PLAYER ? ETileType::OH : ETileType::EX);
				gameState->PassTicTacToeTurn();
			}
		}
	}
}

void ATacBoard::TileChanged(ETileType OldType, ETileType NewType)
{
	// Check for a win based on NewType and if so broadcast gameover(newtype)

	// We also need to check for stalemates, but keep state-altering effects into account if they exist (cards or ammo or whatever form they take)
	// or maybe stalemates should still happen since the alternative is forcing a player to use their effects

	TArray<ATacBoardTile*> tempTiles;
	if (CheckTileRows(NewType, tempTiles) || CheckTileCols(NewType, tempTiles) || CheckDiagonals(NewType, tempTiles))
	{
		switch (NewType)
		{
		case ETileType::OH:
			// Offense wins and the attacking force takes the hex
			OnBoardGameOver.Broadcast(true);
			return;
		case ETileType::EX:
			// Defense wins and the hex stays unchanged
			OnBoardGameOver.Broadcast(false);
			return;
		default:
			break;
		}
	}

	bool anyEmpty = false;
	for (const FTileArray& tileRow : TileRows)
	{
		for (const auto& tile : tileRow.TileCol)
		{
			if (tile->TileType == ETileType::EMPTY)
			{
				anyEmpty = true;
			}
		}
	}

	if (!anyEmpty)
	{
		OnBoardGameOver.Broadcast(false);
	}
}

bool ATacBoard::CheckTileRows(ETileType CheckType, TArray<ATacBoardTile*>& outPotentialWinTiles)
{
	for (const FTileArray& tileRow : TileRows)
	{
		bool rowWin = true;
		int32 numMatched = 0;
		ATacBoardTile* emptyTile = nullptr;;
		for (const auto& tile : tileRow.TileCol)
		{
			if (!tile)
			{
				rowWin = false;
			}
			else if (tile->TileType == ETileType::EMPTY)
			{
				emptyTile = tile;
				rowWin = false;
			}
			else if (tile->TileType != CheckType)
			{
				rowWin = false;
			}
			else
			{
				numMatched++;
			}
		}

		if (numMatched == 2 && IsValid(emptyTile))
		{
			outPotentialWinTiles.Add(emptyTile);
		}

		if (rowWin)
		{
			return true;
		}
	}

	return false;
}

bool ATacBoard::CheckTileCols(ETileType CheckType, TArray<ATacBoardTile*>& outPotentialWinTiles)
{
	if (TileRows.Num() == 0)
	{
		return false;
	}

	for (int32 col = 0; col < TileRows[0].TileCol.Num(); ++col)
	{
		bool colWin = true;
		int32 numMatched = 0;
		ATacBoardTile* emptyTile = nullptr;
		for (int32 row = 0; row < TileRows.Num(); ++row)
		{
			ATacBoardTile* tile = TileRows[row].TileCol[col];
			if (!tile)
			{
				colWin = false;
			}
			else if (tile->TileType == ETileType::EMPTY)
			{
				emptyTile = tile;
				colWin = false;
			}
			else if (tile->TileType != CheckType)
			{
				colWin = false;
			}
			else
			{
				numMatched++;
			}
		}

		if (numMatched == 2 && IsValid(emptyTile))
		{
			outPotentialWinTiles.Add(emptyTile);
		}
		
		if (colWin)
		{
			return true;
		}
	}

	return false;
}

bool ATacBoard::CheckDiagonals(ETileType CheckType, TArray<ATacBoardTile*>& outPotentialWinTiles)
{
	int32 rows = TileRows.Num();
	if (rows <= 0)
	{
		return false;
	}

	int32 cols = TileRows[0].TileCol.Num();

	// Non-square boards would make this weird, so don't check
	if (rows != cols || cols <= 0)
	{
		return false;
	}

	bool diagonalWinA = true;
	int32 numMatched = 0;
	ATacBoardTile* emptyTile = nullptr;;
	for (int32 i = 0; i < rows; ++i)
	{
		ATacBoardTile* tile = TileRows[i].TileCol[i];
		if (!tile)
		{
			diagonalWinA = false;
		}
		else if (tile->TileType == ETileType::EMPTY)
		{
			emptyTile = tile;
			diagonalWinA = false;
		}
		else if (tile->TileType != CheckType)
		{
			diagonalWinA = false;
		}
		else
		{
			numMatched++;
		}
	}
	if (numMatched == 2 && IsValid(emptyTile))
	{
		outPotentialWinTiles.Add(emptyTile);
	}

	bool diagonalWinB = true;
	numMatched = 0;
	emptyTile = nullptr;
	for (int32 i = 0; i < rows; ++i)
	{
		ATacBoardTile* tile = TileRows[i].TileCol[cols - 1 - i];
		if (!tile)
		{
			diagonalWinB = false;
		}
		else if (tile->TileType == ETileType::EMPTY)
		{
			emptyTile = tile;
			diagonalWinB = false;
		}
		else if (tile->TileType != CheckType)
		{
			diagonalWinB = false;
		}
		else
		{
			numMatched++;
		}
	}
	if (numMatched == 2 && IsValid(emptyTile))
	{
		outPotentialWinTiles.Add(emptyTile);
	}

	return diagonalWinA || diagonalWinB;
}


ATacBoardTile* const ATacBoard::GetRandomEmptyTile()
{
	TArray<ATacBoardTile*> emptyTiles;
	for (const FTileArray& tileRow : TileRows)
	{
		for (const auto& tile : tileRow.TileCol)
		{
			if (tile->TileType == ETileType::EMPTY)
			{
				emptyTiles.Add(tile);
			}
		}
	}

	int32 tileIndex = FMath::RandRange(0, emptyTiles.Num() - 1);
	if (emptyTiles.IsValidIndex(tileIndex))
	{
		return emptyTiles[tileIndex];
	}

	return nullptr;
}

TArray<ATacBoardTile*> ATacBoard::GetPotentialWinningTiles()
{
	TArray<ATacBoardTile*> tiles;
	CheckTileRows(ETileType::OH, tiles);
	CheckTileCols(ETileType::OH, tiles);
	CheckDiagonals(ETileType::OH, tiles);
	CheckTileRows(ETileType::EX, tiles);
	CheckTileCols(ETileType::EX, tiles);
	CheckDiagonals(ETileType::EX, tiles);

	return tiles;
}