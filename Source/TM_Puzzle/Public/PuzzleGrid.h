// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleGrid.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	TArray<ATile*> TileGrid;
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	int32 GetIndex(int32 X, int32 Y);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	TSubclassOf<class ATile> TileClass;
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void SpawnTileToGrid(FIntPoint Coordinate);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	int32 GridWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	int32 GridHeight;
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void ChangeTile(ATile* Tile_A, ATile* Tile_B);
	//UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	//void Check();
};
