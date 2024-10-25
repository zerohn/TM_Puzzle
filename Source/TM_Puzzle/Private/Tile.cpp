// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PuzzleGrid.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Tile Collider"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(50.0f));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	MeshComp->SetupAttachment(BoxComp);

	bIsSelected = false;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if(APuzzleGrid* PuzzleGrid = Cast<APuzzleGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleGrid::StaticClass())))
	{
		if(PuzzleGrid->CurrentPuzzleState == EPuzzleState::ResolveBoard)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PopFX, GetActorLocation(), FRotator::ZeroRotator)->SetColorParameter(FxParamName, FxColor);
		}
	}
}

void ATile::SetTileSelected(bool NewValue)
{
	bIsSelected = NewValue;
	if(bIsSelected)
	{
		if(MeshComp)
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("Emissive_Pow"), 100.f);
		}	
	}
	else
	{
		if(MeshComp)
		{
			MeshComp->SetScalarParameterValueOnMaterials(FName("Emissive_Pow"), 0.f);
		}
	}
}

void ATile::MoveToLocation(const FVector& NewLocation, float Duration)
{
	StartLoc = GetActorLocation();
	EndLoc = NewLocation;
	LerpAlpha = 0.f;
	if(APuzzleGrid* PuzzleGrid = Cast<APuzzleGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleGrid::StaticClass())))
	{
		PuzzleGrid->StartMoveAnim();
	}

	GetWorldTimerManager().SetTimer(MoveTimerHandle, this, &ATile::OnMoveAnimUpdate, 0.01f, true);
}

void ATile::OnMoveAnimUpdate()
{
	LerpAlpha += 0.02f;
	FVector NewLocation = FMath::Lerp(StartLoc, EndLoc, LerpAlpha);
	SetActorLocation(NewLocation);

	if(LerpAlpha >= 1.f)
	{
		OnMoveAnimComplete();
	}
}

void ATile::OnMoveAnimComplete()
{
	GetWorldTimerManager().ClearTimer(MoveTimerHandle);
	SetActorLocation(EndLoc);
	SetTileSelected(false);
	
	if(APuzzleGrid* PuzzleGrid = Cast<APuzzleGrid>(UGameplayStatics::GetActorOfClass(GetWorld(), APuzzleGrid::StaticClass())))
	{
		PuzzleGrid->EndMoveAnim();
	}
}
