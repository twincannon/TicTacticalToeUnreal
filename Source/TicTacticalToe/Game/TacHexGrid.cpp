// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacHexGrid.h"
#include "TicTacticalToe/Game/TacGameState.h"

// Sets default values
ATacHexGrid::ATacHexGrid()
{
	HexGridRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HexGridRoot"));
	RootComponent = HexGridRoot;

	PrimaryActorTick.bCanEverTick = false;

	Radius = 50.f;
	GridSize = FIntPoint(5, 5);

	HexWidth = Radius * FMath::Sqrt(3.f);
	HexHeight = Radius * 2.f;
}

// Called when the game starts or when spawned
void ATacHexGrid::BeginPlay()
{
	Super::BeginPlay();

	SetupHexes();
}

void ATacHexGrid::SetupHexes()
{
	bool isFirstHex = true;
	ATacHex* lastHex = nullptr;

	int32 HalfLength = DiamondRadius / 2;
	for (int32 row = 0; row < DiamondRadius; row++)
	{
		int32 numHexes = row <= HalfLength ? (row + 1) : (DiamondRadius - row);
		float offsetX = (HalfLength - numHexes + 1) * (HexWidth * 0.75f);

		for (int32 col = 0; col < numHexes; col++)
		{
			if (HexClass)
			{
				UChildActorComponent* newHex = NewObject<UChildActorComponent>(this);
				newHex->SetChildActorClass(HexClass);
				newHex->SetupAttachment(HexGridRoot);
				newHex->CreateChildActor();

				ATacHex* hex = Cast<ATacHex>(newHex->GetChildActor());
				if (IsValid(hex))
				{
					hex->OnHexClicked.AddDynamic(this, &ATacHexGrid::OnHexClicked);

					hex->HexCoords = FIntPoint(row, col);
					hex->SetDebugText(row, col);

					Hexes.Add(hex);

					if (isFirstHex)
					{
						hex->SetOwnership(EPlayerType::OPPONENT);
						hex->HexHQType = EPlayerType::OPPONENT;
						isFirstHex = false;
					}

					lastHex = hex;

					float x = col * (HexWidth * 1.5f) + offsetX;
					float y = row * (HexHeight * sqrt(3.0f) / 2.0f);
					newHex->SetRelativeLocation(FVector(x, y, 0.f));
				}
			}
		}
	}

	if (lastHex)
	{
		lastHex->SetOwnership(EPlayerType::PLAYER);
		lastHex->HexHQType = EPlayerType::PLAYER;
	}
}

void ATacHexGrid::SetCapturableHexes()
{
	// So row and col both have to be within 1
	for (const auto& hexA : Hexes)
	{
		if (hexA->GetOwningPlayer() != EPlayerType::PLAYER)
		{
			continue;
		}

		TArray<ATacHex*> hexes = hexA->GetOverlappingHexes();
		for (const auto& hex : hexes)
		{
			if (hex->GetOwningPlayer() != EPlayerType::PLAYER && hex->IsNuked() == false)
			{
				hex->SetCapturable(true);
			}
		}
	}
}

void ATacHexGrid::ClearCapturableHexes()
{
	for (const auto& hex : Hexes)
	{
		hex->SetCapturable(false);
	}
}

ATacHex* const ATacHexGrid::GetRandomOpponentCapturableHex()
{
	TArray<ATacHex*> capturableHexes;
	for (const auto& hexA : Hexes)
	{
		if (hexA->GetOwningPlayer() != EPlayerType::OPPONENT)
		{
			continue;
		}

		TArray<ATacHex*> hexes = hexA->GetOverlappingHexes();
		for (const auto& hex : hexes)
		{
			if (hex->GetOwningPlayer() != EPlayerType::OPPONENT && hex->IsNuked() == false)
			{
				capturableHexes.Add(hex);
				hex->DoOpponentCapturableFlash();
			}
		}
	}

	return capturableHexes.Num() > 0 ? capturableHexes[FMath::RandRange(0, capturableHexes.Num() - 1)] : nullptr;
}

void ATacHexGrid::OnHexClicked(ATacHex* const Hex)
{
	if (Hex && Hex->IsCapturable())
	{
		if (ATacGameState* gameState = Cast<ATacGameState>(GetWorld()->GetGameState()))
		{
			gameState->SelectedHex = Hex;
			gameState->OffensiveTeam = EPlayerType::PLAYER; // Player clicked this hex so it must be the player as offensive team
			gameState->DefensiveTeam = Hex->GetOwningPlayer();
			gameState->ChangeState(EGameState::GAME_TICTACTOE);
		}
	}
}
