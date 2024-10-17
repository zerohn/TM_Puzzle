// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStateSubject.h"

UGameStateSubject::UGameStateSubject()
{
	PlayerScore = 0;
}

void UGameStateSubject::RegisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Add(Observer);
}

void UGameStateSubject::UnregisterObserver(TScriptInterface<IObserver> Observer)
{
	Observers.Remove(Observer);
}

void UGameStateSubject::NotifyObservers()
{
	for (TScriptInterface<IObserver> Observer : Observers)
	{
		if(Observer.GetObject() && Observer.GetObject()->GetClass()->ImplementsInterface(UObserver::StaticClass()))
		{
			IObserver::Execute_OnNotify(Observer.GetObject(), PlayerScore);
		}
	}
}

void UGameStateSubject::IncreaseScore(int32 Amount)
{
	PlayerScore += Amount;
	NotifyObservers();
}
