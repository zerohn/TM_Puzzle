// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubject.h"

#include "Puzzle_GameInstance.h"
#include "Kismet/GameplayStatics.h"


UGameStateSubject::UGameStateSubject()
{
}

void UGameStateSubject::RegisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Add(Observer);
}

void UGameStateSubject::UnregisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Remove(Observer);
}

void UGameStateSubject::NotifyObservers(UObject* WorldContextObject)
{
	Observers.RemoveAll([](const TScriptInterface<IObserver>& Observer)
	{
		UObject* Object = Observer.GetObject();
		return Object == nullptr || !IsValid(Object);
	});

	if (!WorldContextObject) return;

	UPuzzle_GameInstance* GameInstance = Cast<UPuzzle_GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	
	if (!GameInstance) return;
	
	int32 CurrentScore = GameInstance->GetCurrentScore();
	
	for (const TScriptInterface<IObserver> Observer : Observers)
	{
		if(IsValid(Observer.GetObject()) && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
		{
			IObserver::Execute_OnNotify(Observer.GetObject(), CurrentScore);
		}
	}
}

void UGameStateSubject::NotifyObserversMoves(UObject* WorldContextObject)
{
	Observers.RemoveAll([](const TScriptInterface<IObserver>& Observer)
	{
		UObject* Object = Observer.GetObject();
		return Object == nullptr || !IsValid(Object);
	});

	if (!WorldContextObject) return;

	UPuzzle_GameInstance* GameInstance = Cast<UPuzzle_GameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
	
	if (!GameInstance) return;

	int32 RemainingMove = GameInstance->GetRemainingMove();

	for (const TScriptInterface<IObserver> Observer : Observers)
	{
		if (IsValid(Observer.GetObject()) && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
		{
			IObserver::Execute_OnNotifyRemainingMoves(Observer.GetObject(), RemainingMove);
		}
	}
}
