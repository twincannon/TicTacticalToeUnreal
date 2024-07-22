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
	UPROPERTY()
	TArray<ATacBoardTile*> TileCol;
};

UCLASS(BlueprintType)
class TICTACTICALTOE_API ATacBoard : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoardGameOver, EPlayerType, Winner);
	
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

	UPROPERTY()
	TArray<FTileArray> TileRows;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CheckTileRows(ETileType CheckType);
	bool CheckTileCols(ETileType CheckType);
	bool CheckDiagonals(ETileType CheckType);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	ATacBoardTile* const GetRandomEmptyTile();

private:
	void SetupTiles();

	UFUNCTION()
	void TileClickedCallback(ATacBoardTile* const Tile);

	UFUNCTION()
	void TileChanged(ETileType OldType, ETileType NewType);

};
