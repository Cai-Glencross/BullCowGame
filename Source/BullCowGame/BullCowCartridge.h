// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct TResults {
	FString KnownWord;
	TSet<TCHAR> KnownLetters;
	TSet<TCHAR> WrongLetters;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	public:
		virtual void BeginPlay() override;
		virtual void OnInput(const FString& Input) override;
		void SetUpGame();
		void EndGame();
		void ProcessGuess(const FString& Guess);
		static bool IsIsogram(const FString& Word);
		TArray<FString> GetValidWords(const TArray<FString>& Words) const;
		void GetBullCows(const FString& Guess, TResults& results);

	// Your declarations go below!
	private:
		FString HiddenWord;
		TResults Results;
		int32 NumLives;
		bool bGameOver;
		TArray<FString> Isograms;
	
};
