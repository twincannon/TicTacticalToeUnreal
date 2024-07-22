// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacBoardTile.h"

// Sets default values
ATacBoardTile::ATacBoardTile()
{
	TileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TileRoot"));
	RootComponent = TileRoot;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(TileRoot);

	TileCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile Collision"));
	TileCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TileCollision->SetCollisionResponseToChannel(COLLISION_CLICK, ECollisionResponse::ECR_Block);
	TileCollision->SetupAttachment(TileRoot);

#if WITH_EDITOR
	TileDebugText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Tile Debug Text"));
	TileDebugText->SetupAttachment(TileRoot);
	TileDebugText->SetText(FText::FromString("Test"));
#endif


 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATacBoardTile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATacBoardTile::SetTileType_Implementation(ETileType NewType)
{
	ETileType oldType = TileType;
	TileType = NewType;

	OnTileTypeChanged.Broadcast(oldType, NewType);
}

// Called every frame
void ATacBoardTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATacBoardTile::InitializeTile(FIntPoint Coords)
{
	TileCoords = Coords;

	SetActorHiddenInGame(false);
}
