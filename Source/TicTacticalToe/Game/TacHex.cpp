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
	HexMesh->SetCollisionObjectType(COLLISION_HEX);
	HexMesh->SetCollisionResponseToChannel(COLLISION_CLICK, ECollisionResponse::ECR_Block);
	HexMesh->SetGenerateOverlapEvents(true);
	HexMesh->SetupAttachment(HexRoot);

	HexDetector = CreateDefaultSubobject<USphereComponent>(TEXT("HexDetector"));
	HexDetector->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HexDetector->SetCollisionResponseToChannel(COLLISION_HEX, ECollisionResponse::ECR_Overlap);
	HexDetector->SetCollisionResponseToChannel(COLLISION_CLICK, ECollisionResponse::ECR_Ignore);
	HexDetector->SetGenerateOverlapEvents(true);
	HexDetector->SetupAttachment(HexRoot);

	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATacHex::BeginPlay()
{
	Super::BeginPlay();
	
	OnBeginCursorOver.AddDynamic(this, &ATacHex::OnMouseOver);
	OnEndCursorOver.AddDynamic(this, &ATacHex::OnMouseLeave);
}

TArray<ATacHex*> ATacHex::GetOverlappingHexes()
{
	TArray<AActor*> outActors;
	HexDetector->GetOverlappingActors(outActors);

	TArray<ATacHex*> hexes;
	for (const auto& actor : outActors)
	{
		if (actor == this)
		{
			continue;
		}

		ATacHex* hex = Cast<ATacHex>(actor);
		if (hex)
		{
			hexes.Add(hex);
		}
	}

	return hexes;
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

void ATacHex::NukeHex_Implementation()
{
	bIsNuked = true;
}