// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacPawn.h"

// Sets default values
ATacPawn::ATacPawn()
{
	PawnRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PawnRoot"));
	RootComponent = PawnRoot;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATacPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->bShowMouseCursor = true;
	}
}

// Called every frame
void ATacPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATacPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Click", IE_Pressed, this, &ATacPawn::DoClickTrace);

}

void ATacPawn::DoClickTrace()
{
	UE_LOG(LogTemp, Warning, TEXT("DoClickTrace"));

	FVector traceEnd, traceDir;
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->DeprojectMousePositionToWorld(traceEnd, traceDir);
		DrawDebugLine(GetWorld(), GetActorLocation(), traceEnd + (traceDir * 1000.f), FColor::Red, false, 5.f);
	}

	
}
