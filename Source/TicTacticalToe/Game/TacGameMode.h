// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TacGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTICALTOE_API ATacGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATacGameMode();

	virtual void StartPlay() override;
};
