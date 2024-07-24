// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TacBoardTile.h"
#include "TacBoard.generated.h"

USTRUCT(BlueprintType)
struct FTileArray
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TArray<ATacBoardTile*> TileCol;
};

UCLASS(BlueprintType)
class TICTACTICALTOE_API ATacBoard : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoardGameOver, bool, DidOffenseWin);
	
public:	
	// Sets default values for this actor's properties
	ATacBoard();
	
	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnBoardGameOver OnBoardGameOver;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	USceneComponent* BoardRoot;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	TSubclassOf<ATacBoardTile> TileClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	FIntPoint BoardSize = FIntPoint(3, 3);

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TArray<FTileArray> TileRows;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CheckTileRows(ETileType CheckType, TArray<ATacBoardTile*>& outPotentialWinTiles);
	bool CheckTileCols(ETileType CheckType, TArray<ATacBoardTile*>& outPotentialWinTiles);
	bool CheckDiagonals(ETileType CheckType, TArray<ATacBoardTile*>& outPotentialWinTiles);


public:	

	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	ATacBoardTile* const GetRandomEmptyTile();

	TArray<ATacBoardTile*> GetPotentialWinningTiles();

	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	TArray<ATacBoardTile*> GetCenterWallTiles();
	
	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	TArray<ATacBoardTile*> GetTilesIn3By3Radius(ATacBoardTile* TargetTile);

	// Clears the row or col from the center wall tile (the only type of tile that you can select that infers a row or col)
	// Returns if the clear was succesful (i.e. a valid target tile was chosen)
	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	bool ClearRowOrColFromCenterWallTile(ATacBoardTile* TargetTile);

	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	void ClearSelectableTiles();

private:
	void SetupTiles();

	UFUNCTION()
	void TileClickedCallback(ATacBoardTile* const Tile);

	UFUNCTION()
	void TileChanged(ETileType OldType, ETileType NewType);

};
