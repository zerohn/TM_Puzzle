// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleGrid.generated.h"

UENUM(BlueprintType)
enum class EPuzzleState : uint8
{
	Main UMETA(DisplayName = "Main"),
	Start UMETA(DisplayName = "Start"),
	Idle UMETA(DisplayName = "Idle"),
	Move UMETA(DisplayName = "Move"),
	Check UMETA(DisplayName = "Check"),
	Pop UMETA(DisplayName = "Pop"),
	Spawn UMETA(DisplayName = "Spawn"),
	End UMETA(DisplayName = "End")
};

class ATile;

UCLASS()
class TM_PUZZLE_API APuzzleGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	EPuzzleState CurrentPuzzleState;
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	void SetPuzzleState(const EPuzzleState NewPuzzleState);
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	void HandlePuzzleState();
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	int32 GetIndex(int32 X, int32 Y);
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	FIntPoint GetCoordinate(int32 idx);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	TArray<TSubclassOf<class ATile>> TileClass;
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	TSubclassOf<class ATile> GetRandomTileClass();
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void InitPuzzleGrid();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	TArray<ATile*> PuzzleGrid;
	ATile* SelectedTile[2];
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	void AddSelectedTile(ATile* NewTile);
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	bool bCanChangeTile(ATile* Tile_A, ATile* Tile_B);
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void ChangeTile();
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void ChangeAnimation(const FVector Loc_A, const FVector Loc_B);
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void UpdateTileIndex();
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void SpawnTileToGrid(FIntPoint Coordinate);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	int32 GridWidth = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	int32 GridHeight = 8;
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	bool MatchingCheck();
	TArray<int32> MatchingTileIdx;
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void PopTile();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	class ATileCommandInvoker* CommandInvoker;
	FTimerHandle TileAnimHandle;
	float Alpha;
};
