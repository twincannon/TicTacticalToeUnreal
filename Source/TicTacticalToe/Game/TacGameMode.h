// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TacGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTICALTOE_API ATacGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	bool IsPlayersTurn() { return bIsPlayersTurn; }

	virtual void BeginPlay() override;

protected:
	bool bIsPlayersTurn = true;
};
