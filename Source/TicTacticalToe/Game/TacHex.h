// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacticalToe/TicTacticalToe.h"
#include <Components/SphereComponent.h>
#include "TacHex.generated.h"


UCLASS()
class TICTACTICALTOE_API ATacHex : public AActor
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHexClicked, ATacHex* const, Hex);
	
public:	
	// Sets default values for this actor's properties
	ATacHex();

	UPROPERTY(BlueprintAssignable, Category = "TicTacticalToe")
	FOnHexClicked OnHexClicked;

	UFUNCTION(BlueprintImplementableEvent)
	void SetDebugText(int32 Row, int32 Col);

	UFUNCTION(BlueprintNativeEvent, Category = "TicTacticalToe")
	void SetOwnership(EPlayerType Player);
	virtual void SetOwnership_Implementation(EPlayerType Player);

	EPlayerType GetOwningPlayer() { return OwningPlayer; }

	UPROPERTY(BlueprintReadOnly, Category = "TicTacticalToe")
	FIntPoint HexCoords;

	UFUNCTION(BlueprintNativeEvent, Category = "TicTacticalToe")
	void SetCapturable(bool bIsCapturable);
	virtual void SetCapturable_Implementation(bool bIsCapturable);

	UFUNCTION(BlueprintPure, Category = "TicTacticalToe")
	bool IsCapturable() { return bCapturable; }

	UFUNCTION(BlueprintImplementableEvent, Category = "TicTacticalToe")
	void OnHexChosen();

	UFUNCTION(BlueprintImplementableEvent, Category = "TicTacticalToe")
	void DoOpponentCapturableFlash();

	TArray<ATacHex*> GetOverlappingHexes();

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	USceneComponent* HexRoot;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	UStaticMeshComponent* HexMesh;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "TicTacticalToe")
	USphereComponent* HexDetector;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bCapturable = false;

	EPlayerType OwningPlayer = EPlayerType::NEUTRAL;
	
	UFUNCTION(BlueprintNativeEvent, Category = "TicTacticalToe")
	void OnMouseOver(AActor* Actor);
	virtual void OnMouseOver_Implementation(AActor* Actor);

	UFUNCTION(BlueprintNativeEvent, Category = "TicTacticalToe")
	void OnMouseLeave(AActor* Actor);
	virtual void OnMouseLeave_Implementation(AActor* Actor);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
