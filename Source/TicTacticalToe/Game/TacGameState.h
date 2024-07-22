// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TicTacticalToe/TicTacticalToe.h"
#include "TicTacticalToe/Game/TacBoard.h"
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
	bool IsPlayersTurn() { return bIsPlayersTurn; }

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnStateChanged OnStateChanged;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TEnumAsByte<EGameState> CurrentState;

	void PassTurn();

	UFUNCTION(BlueprintCallable, Category = "TicTacticalToe")
	void ChangeState(EGameState NewState);

	// Board class to be defined in child blueprint
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	TSubclassOf<ATacBoard> BoardClass;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	ATacBoard* Board = nullptr;

protected:
	bool bIsPlayersTurn = true;

	UFUNCTION()
	void OnBoardGameOver(EPlayerType Winner);

private:
	FTimerHandle OpponentTurnTimer;

	UFUNCTION()
	void OpponentTurn();
};
