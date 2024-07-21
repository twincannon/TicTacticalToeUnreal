// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacBoard.h"

// Sets default values
ATacBoard::ATacBoard()
{
	BoardRoot = CreateDefaultSubobject<USceneComponent>(TEXT("BoardRoot"));
	RootComponent = BoardRoot;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATacBoard::BeginPlay()
{
	Super::BeginPlay();
	
	SetupTiles();
}

// Called every frame
void ATacBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATacBoard::SetupTiles()
{
	for (int32 i = 0; i < 3; ++i)
	{
		for (int32 j = 0; j < 3; ++j)
		{
			UChildActorComponent* newTile = NewObject<UChildActorComponent>(this);
			newTile->SetChildActorClass(TileClass);
			newTile->SetupAttachment(BoardRoot);
			newTile->CreateChildActor();

			ATacBoardTile* tile = Cast<ATacBoardTile>(newTile->GetChildActor());
			if (IsValid(tile))
			{
				tile->SetOwner(this);
				tile->SetActorHiddenInGame(false);
				tile->OnTileClicked.AddDynamic(this, &ATacBoard::TileClickedCallback);
			}

			newTile->SetRelativeLocation(FVector(TileSpacing * i, TileSpacing * j, 0.f));
		}
	}
}

void ATacBoard::TileClickedCallback(ATacBoardTile* const tile)
{
	UE_LOG(LogTemp, Warning, TEXT("Tile clicked"));
}
