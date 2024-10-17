// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePawn.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "PuzzleGrid.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APuzzlePawn::APuzzlePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APuzzlePawn::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubSystem = UGameplayStatics::GetPlayerController(this, 0)->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(InputSubSystem)
	{
		InputSubSystem->AddMappingContext(IMC_Puzzle, 0);
	}
}

// Called every frame
void APuzzlePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APuzzlePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(Input)
	{
		Input->BindAction(IA_Click, ETriggerEvent::Started, this, &APuzzlePawn::OnClick);
	}
}

void APuzzlePawn::OnClick(const FInputActionValue& value)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Click");
	float ScreenX, ScreenY;
	if(UGameplayStatics::GetPlayerController(this, 0)->GetMousePosition(ScreenX, ScreenY))
	{
		FVector WorldLoc, WorldDir;
		UGameplayStatics::GetPlayerController(this, 0)->DeprojectScreenPositionToWorld(ScreenX, ScreenY, WorldLoc, WorldDir);
		FHitResult Hit;
		FVector StartLoc = WorldLoc;
		FVector EndLoc = WorldLoc + WorldDir * 3000.f;
		if(GetWorld()->LineTraceSingleByChannel(Hit, StartLoc, EndLoc, ECC_Visibility))
		{
			if(ATile* Tile = Cast<ATile>(Hit.GetActor()))
			{
				APuzzleGrid* PuzzleGrid = Cast<APuzzleGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleGrid::StaticClass()));
				if(PuzzleGrid)
				{
					PuzzleGrid->AddSelectedTile(Tile);
				}
			}
		}
	}
}

