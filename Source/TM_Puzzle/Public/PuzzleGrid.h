// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "GameFramework/Actor.h"
#include "PuzzleGrid.generated.h"

UENUM(BlueprintType)
enum class EPuzzleState : uint8
{
	MainMenu UMETA(DisplayName = "MainMenu"),
	Initialize UMETA(DisplayName = "Initialize"),
	PlayerTurn UMETA(DisplayName = "PlayerTurn"),
	ResolveBoard UMETA(DisplayName = "ResolveBoard"),
	GameOver UMETA(DisplayName = "GameOver")
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

	// 퍼즐 FSM 상태 변수
	EPuzzleState CurrentPuzzleState;
	// 퍼즐의 현재 State Set 함수
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	void SetPuzzleState(const EPuzzleState NewPuzzleState);
	// 퍼즐의 State에 따라 동작하는 메인 Logic 실행 함수
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	void HandlePuzzleState();
	// 퍼즐의 X, Y 좌표를 Index로 반환해주는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	int32 GetIndex(int32 X, int32 Y);
	// 퍼즐의 Index를 X, Y(FIntpoint)형태로 반환해주는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	FIntPoint GetCoordinate(int32 idx);
	// 퍼즐에 존재하는 타일의 종류를 담아둔 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	TArray<TSubclassOf<class ATile>> TileClass;
	// 랜덤하게 타일 클래스를 반환해주는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	TSubclassOf<class ATile> GetRandomTileClass();
	// 퍼즐의 타일들을 저장하는 배열, 그리드
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	TArray<ATile*> PuzzleGrid;
	// 타일들의 간격 조정을 위한 사이즈
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	float TileSize = 100.f;
	// 퍼즐의 그리드에 초기 타일을 배치하는 게임 초기화 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void InitPuzzleGrid();
	// 선택된 타일을 담아두는 변수
	ATile* SelectedTile[2];
	// 선택된 타일을 추가하는 함수
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	void AddSelectedTile(ATile* NewTile);
	// 선택된 두 타일이 자리를 변경할 수 있는지 확인하는 함수
	UFUNCTION(BlueprintCallable, Category = "PuzzleGrid")
	bool bCanSwapTile(ATile* Tile_A, ATile* Tile_B);
	// 선택된 두 타일의 그리드 Index를 교환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void SwapTile(ATile* Tile_A, ATile* Tile_B);
	// 선택된 두 타일의 자리를 교환하는 Animation 함수
	// UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	// void ChangeAnimation(ATile* Tile_A, ATile* Tile_B, const FVector Loc_A, const FVector Loc_B);
	// 그리드의 빈 공간으로 타일을 이동 시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void DropTile();
	// 
	// 그리드의 빈 Index에 새 타일을 생성하는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void SpawnTileToGrid();
	// 그리드의 가로, 세로 사이즈를 정하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	int32 GridWidth = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	int32 GridHeight = 8;
	// 3개 이상 연결된 타일이 존재하는지 확인하는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	bool MatchingCheck();
	// 매칭된 타일들의 Index를 저장하는 배열
	TArray<int32> MatchingTileIdx;
	// 매칭된 타일을 소멸시키는 함수
	UFUNCTION(BlueprintCallable, Category = "Puzzle Grid")
	void PopTile();
	// Command 패턴을 사용할 수 있는 Manager Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Puzzle Grid")
	class ATileCommandInvoker* CommandInvoker;
	// 타일의 Animation에 필요한 Handle과 Alpha 변수 
	FTimerHandle TileAnimHandle;
	float Alpha;
	// 타일의 Animation 동작을 확인할 수 있는 Counter, 멀티스레드에서 Counting 안전성 보장
	FThreadSafeCounter MoveCounter;
	// MoveCounter 증감 함수
	UFUNCTION()
	void StartMoveAnim();
	UFUNCTION()
	void EndMoveAnim();
};
