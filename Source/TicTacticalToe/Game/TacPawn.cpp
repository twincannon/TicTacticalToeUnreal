// Fill out your copyright notice in the Description page of Project Settings.


#include "TicTacticalToe/Game/TacPawn.h"
#include "TicTacticalToe/Game/TacBoardTile.h"
#include "TicTacticalToe/Game/TacGameState.h"
#include "TicTacticalToe/Game/TacHex.h"

#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ATacPawn::ATacPawn()
{
	PawnRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PawnRoot"));
	RootComponent = PawnRoot;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(PawnRoot);
	CameraComponent->SetAutoActivate(true);

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetBPClass(TEXT("/Game/UI/UI_MainMenu"));
	if (WidgetBPClass.Class != nullptr)
	{
		MainMenuWidgetClass = WidgetBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = false;
}

void ATacPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (ATacGameState* gameState = Cast<ATacGameState>(GetWorld()->GetGameState()))
	{
		gameState->OnStateChanged.AddDynamic(this, &ATacPawn::HandleStateChanged);
	}
}

void ATacPawn::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->bShowMouseCursor = true;
		pc->bEnableMouseOverEvents = true;
		//pc->SetInputMode(FInputModeGameAndUI()); // For some reason this causes the cursor to disappear when clicking
	}
}

void ATacPawn::HandleStateChanged(EGameState NewState)
{
	switch (NewState)
	{
	case EGameState::MAINMENU:
		ShowMainMenu();
		break;
	case EGameState::GAME_OVERVIEW:
		break;
	case EGameState::GAME_TICTACTOE:
		break;
	case EGameState::GAMEOVER:
		break;
	default:
		break;
	}
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

// Called to bind functionality to input
void ATacPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Click", IE_Pressed, this, &ATacPawn::DoClickTrace);
	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &ATacPawn::ExitGame);

}

void ATacPawn::DoClickTrace()
{
	FVector mousePos, mouseDir;
	if (APlayerController* pc = Cast<APlayerController>(GetController()))
	{
		pc->DeprojectMousePositionToWorld(mousePos, mouseDir);
		FVector traceStart = CameraComponent->GetComponentLocation();
		FVector traceEnd = mousePos + (mouseDir * 5000.f);
		FHitResult hit;
		DrawDebugLine(GetWorld(), traceStart, traceEnd, FColor::Red, false, 5.f);
		GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, COLLISION_CLICK);

		if (ATacBoardTile* tile = Cast<ATacBoardTile>(hit.GetActor()))
		{
			tile->OnTileClicked.Broadcast(tile);
		}
		else if (ATacHex* hex = Cast<ATacHex>(hit.GetActor()))
		{
			hex->OnHexClicked.Broadcast(hex);
		}
	}	
}

void ATacPawn::ExitGame()
{
	FGenericPlatformMisc::RequestExit(false);
}
