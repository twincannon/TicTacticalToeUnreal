// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TicTacticalToe/Game/TacGameState.h"
#include "TacItem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TICTACTICALTOE_API UTacItem : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	FText ItemName;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	FText ItemDescription;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	UTexture2D* ItemTexture;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TicTacticalToe")
	void ExecuteItem(ATacGameState* GameState);
};
