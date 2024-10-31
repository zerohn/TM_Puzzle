// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM(BlueprintType)
enum class ETileType : uint8
{
	Sun UMETA(DisplayName = "Sun"),
	Moon UMETA(DisplayName = "Moon"),
	Earth UMETA(DisplayName = "Earth"),
	Jupiter UMETA(DisplayName = "Jupiter"),
	Mars UMETA(DisplayName = "Mars"),
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	// virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	class UBoxComponent* BoxComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	class UStaticMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	UNiagaraSystem* PopFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FName FxParamName = "Pop Color";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FLinearColor FxColor = FColor::White;
	
protected:
	// 타일의 Type 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	ETileType TileType;
	
public:
	UFUNCTION(BlueprintCallable, Category = "Tile")
	ETileType GetTileType() { return TileType; }
	UFUNCTION(BlueprintCallable, Category = "Tile")
	void SetTileSelected(bool NewValue);
	// UFUNCTION(BlueprintCallable, Category = "Tile")
	// void ChangeTileSelected(bool bIsSelected);
	bool bIsSelected;
	bool bIsMatched;
	UFUNCTION()
	void MoveToLocation(const FVector& NewLocation, float Duration);
	
private:
	// 타일 Animation 변수, 함수
	FTimerHandle MoveTimerHandle;
	FVector StartLoc;
	FVector EndLoc;
	float LerpAlpha;
	UFUNCTION()
	void OnMoveAnimUpdate();
	UFUNCTION()
	void OnMoveAnimComplete();
	
};
