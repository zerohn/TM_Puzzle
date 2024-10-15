// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Sun UMETA(DisplayName = "Sun"),
	Moon UMETA(DisplayName = "Moon"),
	Earth UMETA(DisplayName = "Earth"),
	Jupiter UMETA(DisplayName = "Jupiter"),
	MaxTileType
};

UCLASS()
class TM_PUZZLE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	ETileType TileType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TArray<UStaticMesh*> TileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TArray<UMaterialInstance*> TileMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 CoordX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 CoordY;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 ConnectedTile;
	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetTileSelected();
	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetCoord(FIntPoint NewCoord);
	UFUNCTION(BlueprintCallable, Category = "Tile")
	FIntPoint GetCoord();
	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetTileLocation(FIntPoint NewCoord);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	float SlideDuration;
	float CurrentTime;
	bool bIsSliding;
	FVector CurrentLoc, ToMoveLoc;
	bool bIsSelected;
};
