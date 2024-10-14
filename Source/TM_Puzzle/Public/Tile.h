// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	
}

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
	FColor ??;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 CoordX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 CoordY;

	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetCoord(int32 X, int32 Y);
};
