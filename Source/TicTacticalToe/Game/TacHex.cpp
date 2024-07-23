// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacHex.h"
#include "TicTacticalToe/Game/TacGameState.h"

// Sets default values
ATacHex::ATacHex()
{
	HexRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HexRoot"));
	RootComponent = HexRoot;

	HexMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HexMesh"));
	HexMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HexMesh->SetCollisionResponseToChannel(COLLISION_CLICK, ECollisionResponse::ECR_Block);
	HexMesh->SetupAttachment(HexRoot);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

// Called when the game starts or when spawned
void ATacHex::BeginPlay()
{
	Super::BeginPlay();
	
	OnBeginCursorOver.AddDynamic(this, &ATacHex::OnMouseOver);
	OnEndCursorOver.AddDynamic(this, &ATacHex::OnMouseLeave);
}

// Called every frame
void ATacHex::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATacHex::SetOwnership_Implementation(EPlayerType Player)
{
	OwningPlayer = Player;
}

void ATacHex::OnMouseOver_Implementation(AActor* Actor)
{
	if (ATacGameState* gameState = Cast<ATacGameState>(GetWorld()->GetGameState()))
	{
		// Is this hex a neighbor of something we own? we should highlight any hex we hover over, actually?
	}
}

void ATacHex::OnMouseLeave_Implementation(AActor* Actor)
{

}

void ATacHex::SetCapturable_Implementation(bool bIsCapturable)
{
	bCapturable = bIsCapturable;
}