// Fill out your copyright notice in the Description page of Project Settings.

PRAGMA_DISABLE_OPTIMIZATION

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

				float tileSpacing = tile->GetTileSpacing();
				newTile->SetRelativeLocation(FVector(tileSpacing* i, tileSpacing* j, 0.f));
			}
		}
	}
}

void ATacBoard::TileClickedCallback(ATacBoardTile* const Tile)
{
	UE_LOG(LogTemp, Warning, TEXT("Tile clicked"));
	if (ATacGameState* gameState = Cast<ATacGameState>(GetWorld()->GetGameState()))
	{
		if (gameState->IsPlayersTurn())
		{
			Tile->SetTileType(ETileType::OH);
			gameState->PassTurn();
		}
	}
}

void ATacBoard::TileChanged(ETileType OldType, ETileType NewType)
{
	// Check for a win based on NewType and if so broadcast gameover(newtype)

	// We also need to check for stalemates, but keep state-altering effects into account if they exist (cards or ammo or whatever form they take)
	// or maybe stalemates should still happen since the alternative is forcing a player to use their effects

	if (CheckTileRows(NewType) || CheckTileCols(NewType) || CheckDiagonals(NewType))
	{
		switch (NewType)
		{
		case ETileType::OH:
			OnBoardGameOver.Broadcast(EPlayerType::PLAYER);
			break;
		case ETileType::EX:
			OnBoardGameOver.Broadcast(EPlayerType::OPPONENT);
			break;
		default:
			break;
		}
	}


}

bool ATacBoard::CheckTileRows(ETileType CheckType)
{
	for (const FTileArray& tileRow : TileRows)
	{
		bool rowWin = true;
		for (const auto& tile : tileRow.TileCol)
		{
			if (!tile || tile->TileType != CheckType)
			{
				rowWin = false;
				break;
			}
		}

		if (rowWin)
		{
			return true;
		}
	}

	return false;
}

bool ATacBoard::CheckTileCols(ETileType CheckType)
{
	if (TileRows.Num() == 0)
	{
		return false;
	}

	for (int32 col = 0; col < TileRows[0].TileCol.Num(); ++col)
	{
		bool colWin = true;
		for (int32 row = 0; row < TileRows.Num(); ++row)
		{
			ATacBoardTile* tile = TileRows[row].TileCol[col];
			if (!tile || tile->TileType != CheckType)
			{
				colWin = false;
				break;
			}
		}
		
		if (colWin)
		{
			return true;
		}
	}

	return false;
}

bool ATacBoard::CheckDiagonals(ETileType CheckType)
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

	bool diagonalWin = true;
	for (int32 i = 0; i < rows; ++i)
	{
		ATacBoardTile* tile = TileRows[i].TileCol[i];
		if (!tile || tile->TileType != CheckType)
		{
			diagonalWin = false;
			break;
		}
	}

	diagonalWin = true;
	for (int32 i = 0; i < rows; ++i)
	{
		ATacBoardTile* tile = TileRows[i].TileCol[cols - 1 - i];
		if (!tile || tile->TileType != CheckType)
		{
			diagonalWin = false;
			break;
		}
	}

	return diagonalWin;
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

PRAGMA_ENABLE_OPTIMIZATION