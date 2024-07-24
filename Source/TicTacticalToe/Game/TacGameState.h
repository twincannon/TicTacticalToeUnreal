// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TicTacticalToe/TicTacticalToe.h"
#include "TicTacticalToe/Game/TacBoard.h"
#include "TicTacticalToe/Game/TacHex.h"
#include "TicTacticalToe/Game/TacHexGrid.h"
#include "TacGameState.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTICALTOE_API ATacGameState : public AGameState
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, EGameState, NewState);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemTileSelected, ATacBoardTile*, SelectedTile);


public:
	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	bool IsPlayersTicTacToeTurn() { return bIsPlayersTicTacToeTurn; }

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnStateChanged OnStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnItemTileSelected OnItemTileSelected;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TEnumAsByte<EGameState> CurrentState;

	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	void PassTicTacToeTurn();

	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	void ChangeState(EGameState NewState);

	// Board class to be defined in child blueprint
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	TSubclassOf<ATacBoard> BoardClass;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	ATacBoard* Board = nullptr;

	// Should probably be a soft pointer
	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	ATacHex* SelectedHex = nullptr;

	ATacHexGrid* GetHexGrid();

	bool bIsTicTacToeOffenseTurn = true; // Offense always goes first

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TEnumAsByte<EPlayerType> OffensiveTeam = EPlayerType::NONE;
	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TEnumAsByte<EPlayerType> DefensiveTeam = EPlayerType::NONE;

	// Winning team of the entire campaign
	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TEnumAsByte<EPlayerType> WinningTeam = EPlayerType::NONE;

	UFUNCTION(BlueprintImplementableEvent, Category = "TicTacticalToe")
	void OnPlayerWonHex();

	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	void OnBoardGameOver(bool DidOffenseWin);

	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	bool IsBoardGameOver();

	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	bool IsSelectingTileForItem() { return bIsSelectingTileForItem; }
	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	void SetSelectingTileForItem(bool bIsSelecting);

protected:

	UPROPERTY()
	ATacHexGrid* HexGrid;

	bool bIsPlayersTicTacToeTurn = true;

	EPlayerType TicTacToeTeamTurn = EPlayerType::NEUTRAL;
	
	bool bIsPlayersHexTurn = false; // We flip this at the start of overview state so start out false

	bool bIsSelectingTileForItem = false;

	float TurnTimer = 0.75f;

	UFUNCTION()
	void DestroyBoard();

	UFUNCTION()
	void SetCapturableHexesTimerDone();

	UFUNCTION()
	void OpponentChooseHex();

	UFUNCTION()
	void OpponentConfirmHex();

private:
	FTimerHandle OpponentTicTacToeTurnTimer;
	FTimerHandle BoardGameOverTimer;

	UFUNCTION()
	void OpponentTicTacToeTurn();

	UFUNCTION()
	void NeutralTicTacToeTurn();
};
