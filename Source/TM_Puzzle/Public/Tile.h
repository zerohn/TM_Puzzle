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
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	ETileType TileType;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Tile")
	ETileType GetTileType() { return TileType; }

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void ChangeTileSelected();

	bool bIsSelected;
	bool bIsMatched;
};
