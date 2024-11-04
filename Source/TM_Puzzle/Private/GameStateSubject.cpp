// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubject.h"

#include "Puzzle_GameInstance.h"
#include "EntitySystem/MovieSceneComponentDebug.h"
#include "Kismet/GameplayStatics.h"


UGameStateSubject::UGameStateSubject()
{
}

void UGameStateSubject::RegisterObserver(TScriptInterface<IObserver> Observer)
{
	FScopeLock Lock(&CriticalSection);
	if (Observer.GetObject() || IsValid(Observer.GetObject()))
	{
		Observers.Add(Observer);
		UE_LOG(LogTemp, Warning, TEXT("Observer Registered : %s"), *Observer.GetObject()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to register invalid Observer."));
	}
}

void UGameStateSubject::UnregisterObserver(TScriptInterface<IObserver> Observer)
{
	FScopeLock Lock(&CriticalSection);
	if (Observers.Remove(Observer) > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Observer Unregistered : %s"), *Observer.GetObject()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Attempted to unregister an Observer that was not registered."));
	}
}

void UGameStateSubject::NotifyObservers(UObject* WorldContextObject)
{
	if (!WorldContextObject) return;

	// 동기화된 접근을 위해 Critical Section 사용
	FScopeLock Lock(&CriticalSection);
    
	// 유효하지 않은 Observer 제거
	for (TScriptInterface<IObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			UnregisterObserver(Observer);
		}
	}
	
	// Observers.RemoveAll([](const TScriptInterface<IObserver>& Observer)
	// {
	// 	if (!Observer.IsValid())
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Removing invalid Observer"));
	// 		return true;
	// 	}
	// 	return false;
	// });

	// GameInstance 가져오기
	UPuzzle_GameInstance* GameInstance = Cast<UPuzzle_GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GameInstance) return;

	int32 CurrentScore = GameInstance->GetCurrentScore();

	// 유효한 Observer에게 알림 보내기
	for (const TScriptInterface<IObserver>& Observer : Observers)
	{
		if (Observer && IsValid(Observer.GetObject()))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnNotify Observed : %s"), *Observer.GetObject()->GetName());
			IObserver::Execute_OnNotify(Observer.GetObject(), CurrentScore);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skipped notifying invalid Observer."));
		}
	}
}

void UGameStateSubject::NotifyObserversMoves(UObject* WorldContextObject)
{
	if (!WorldContextObject) return;

	// 동기화된 접근을 위해 Critical Section 사용
	FScopeLock Lock(&CriticalSection);
    
	// 유효하지 않은 Observer 제거
	for (TScriptInterface<IObserver> Observer : Observers)
	{
		if (!IsValid(Observer.GetObject()))
		{
			UnregisterObserver(Observer);
		}
	}
	// Observers.RemoveAll([](const TSharedPtr<IObserver>& Observer)
	// {
	// 	if (!Observer.IsValid())
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("Removing invalid Observer."));
	// 		return true;
	// 	}
	// 	return false;
	// });

	// GameInstance 가져오기
	UPuzzle_GameInstance* GameInstance = Cast<UPuzzle_GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	if (!GameInstance) return;

	int32 RemainingMove = GameInstance->GetRemainingMove();

	// 유효한 Observer에게 알림 보내기
	for (const TScriptInterface<IObserver>& Observer : Observers)
	{
		if (Observer && IsValid(Observer.GetObject()))
		{
			UE_LOG(LogTemp, Warning, TEXT("OnNotify Remaining Move Observed : %s"), *Observer.GetObject()->GetName());
			IObserver::Execute_OnNotifyRemainingMoves(Observer.GetObject(), RemainingMove);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Skipped notifying invalid Observer."));
		}
	}
}
