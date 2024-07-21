// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TicTacticalToe/TicTacticalToe.h"
#include <Camera/CameraComponent.h>
#include "TacPawn.generated.h"

UCLASS()
class TICTACTICALTOE_API ATacPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATacPawn();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	USceneComponent* PawnRoot;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	UCameraComponent* CameraComponent;


	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;

	UFUNCTION()
	void HandleStateChanged(EGameState NewState);

	void ShowMainMenu();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void DoClickTrace();

};
