// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "TicTacticalToe/TicTacticalToe.h"
#include <Components/BoxComponent.h>
#include <Components/TextRenderComponent.h>

#include "TacBoardTile.generated.h"

UCLASS(BlueprintType)
class TICTACTICALTOE_API ATacBoardTile : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTileClicked, ATacBoardTile* const, Tile);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTileTypeChanged, ETileType, OldType, ETileType, NewType);
		
public:	
	// Sets default values for this actor's properties
	ATacBoardTile();

	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnTileClicked OnTileClicked;

	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnTileTypeChanged OnTileTypeChanged;

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	TEnumAsByte<ETileType> TileType = ETileType::EMPTY;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "TicTacticalToe")
	void SetTileType(ETileType NewType);
	virtual void SetTileType_Implementation(ETileType NewType);

	float GetTileSpacing() { return TileSpacing; }

	void InitializeTile(FIntPoint Coords);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "TicTacticalToe")
	void SetTileSelectable(bool bIsSelectable);

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	USceneComponent* TileRoot;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	UStaticMeshComponent* TileMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	UBoxComponent* TileCollision;

	// Editor only but UPROPERTY fields cannot be hidden behind preprocessor macros
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	UTextRenderComponent* TileDebugText;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	float TileSpacing = 100.f;

	FIntPoint TileCoords;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
