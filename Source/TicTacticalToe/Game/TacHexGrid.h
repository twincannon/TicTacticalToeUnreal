// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacticalToe/Game/TacHex.h"
#include "TacHexGrid.generated.h"

UCLASS()
class TICTACTICALTOE_API ATacHexGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATacHexGrid();

	void SetCapturableHexes();
	void ClearCapturableHexes();

	ATacHex* const GetRandomOpponentCapturableHex();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetupHexes();

	UFUNCTION()
	void OnHexClicked(ATacHex* const Hex);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	int32 DiamondRadius = 5;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	TSubclassOf<ATacHex> HexClass;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	USceneComponent* HexGridRoot;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacticalToe")
	float Radius;

	UPROPERTY(EditDefaultsOnly, Category = "TicTacticalToe")
	FIntPoint GridSize;

	float HexWidth, HexHeight;

	UPROPERTY()
	TArray<ATacHex*> Hexes;

};
