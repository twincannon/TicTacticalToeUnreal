// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TicTacticalToe/TicTacticalToe.h"
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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	EPlayerType OwningPlayer = EPlayerType::NONE;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
