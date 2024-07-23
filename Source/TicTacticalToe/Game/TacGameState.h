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

public:
	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	bool IsPlayersTicTacToeTurn() { return bIsPlayersTicTacToeTurn; }

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnStateChanged OnStateChanged;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TEnumAsByte<EGameState> CurrentState;

	void PassTicTacToeTurn();

	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	void ChangeState(EGameState NewState);

	// Board class to be defined in child blueprint
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	TSubclassOf<ATacBoard> BoardClass;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	ATacBoard* Board = nullptr;

	UPROPERTY()
	TSoftObjectPtr<ATacHex> SelectedHex = nullptr;

	ATacHexGrid* GetHexGrid();

	bool bIsTicTacToeOffenseTurn = true; // Offense always goes first
	EPlayerType OffensiveTeam = EPlayerType::NEUTRAL;
	EPlayerType DefensiveTeam = EPlayerType::NEUTRAL;

protected:

	UPROPERTY()
	ATacHexGrid* HexGrid;

	bool bIsPlayersTicTacToeTurn = true;

	EPlayerType TicTacToeTeamTurn = EPlayerType::NEUTRAL;
	
	bool bIsPlayersHexTurn = false; // We flip this at the start of overview state so start out false

	UFUNCTION()
	void OnBoardGameOver(bool DidOffenseWin);

	UFUNCTION()
	void DestroyBoard();

	UFUNCTION()
	void OpponentChooseHex();

private:
	FTimerHandle OpponentTicTacToeTurnTimer;

	UFUNCTION()
	void OpponentTicTacToeTurn();

	UFUNCTION()
	void NeutralTicTacToeTurn();
};
