// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacPawn.h"
#include "TicTacticalToe/TicTacticalToe.h"
#include "TicTacticalToe/Game/TacBoardTile.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
// Sets default values
ATacPawn::ATacPawn()
{
	PawnRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PawnRoot"));
	RootComponent = PawnRoot;

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBPClass(TEXT("/Game/UI/UI_MainMenu"));
	if (WidgetBPClass.Class != nullptr)
	{
		MainMenuWidgetClass = WidgetBPClass.Class;
	}

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

	ShowMainMenu();
}

void ATacPawn::ShowMainMenu()
{
	if (MainMenuWidgetClass)
	{
		UUserWidget* mainMenu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (IsValid(mainMenu))
		{
			mainMenu->AddToViewport();
		}
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

	FVector mousePos, mouseDir;
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->DeprojectMousePositionToWorld(mousePos, mouseDir);
		FVector traceStart = GetActorLocation();
		FVector traceEnd = mousePos + (mouseDir * 5000.f);
		FHitResult hit;
		DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, 5.f);
		GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, COLLISION_CLICK);

		if (ATacBoardTile* tile = Cast<ATacBoardTile>(hit.GetActor()))
		{
			tile->OnTileClicked.Broadcast(tile);
		}
	}

	
}
