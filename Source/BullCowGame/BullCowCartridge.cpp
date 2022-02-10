// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Containers/Set.h"

using namespace std;

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString HiddenWordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *HiddenWordListPath, [](const FString& Word){
        return Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word);
    });

    SetUpGame();
}

void UBullCowCartridge::SetUpGame()
{
    //Set Word
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()-1)];
    Results.KnownWord = TEXT("");
    for (TCHAR character : HiddenWord) {
        Results.KnownWord += TEXT("_");
    }
    Results.KnownLetters.Empty();
    Results.WrongLetters.Empty();
    //Set Lives
    NumLives = HiddenWord.Len()+1;
    //Set Game Over
    bGameOver = false;

    //Print Instructions
    //PrintLine(FString::Printf(TEXT("Word is %s"), *HiddenWord)); //debug line
    PrintLine(TEXT("Welcome to the Bull Cow Game!!!"));
    PrintLine(TEXT("There is a hidden word for you to guess."));
    PrintLine(TEXT("Press the \"tab\" key to type on this sign. Press Enter to submit your guess."));
    PrintLine(TEXT("The word is an isogram (has no repeating letters)"));
    PrintLine(FString::Printf(TEXT("Guess the %i letter word"), HiddenWord.Len()));
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    //if Game Is Over, call ClearScreen() and call SetUpGame()
    if (bGameOver) {
        ClearScreen();
        SetUpGame();
        return;
    }
    ProcessGuess(Input);
}

void UBullCowCartridge::EndGame() {
    bGameOver = true;
    PrintLine(TEXT("Press Enter To Continue..."));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess) {
    FString LowerGuess = Guess.ToLower();
    if (LowerGuess.Equals(HiddenWord)) {
        PrintLine(TEXT("Correct! nice work!"));
        EndGame();
        return;
    }

    //check number of characters
    if (HiddenWord.Len() != Guess.Len()) {
        PrintLine(TEXT("The hidden word is %i characters long! Try again."), HiddenWord.Len());
    }
    //check isogram
    else if (!IsIsogram(LowerGuess)){
        PrintLine(TEXT("Your guess was not an isogram, try again."));
    }
    else if (--NumLives <= 0) {
        PrintLine(TEXT("You have run out of lives!!! The word was %s"), *HiddenWord);
        EndGame();
    } else {
        GetBullCows(LowerGuess, Results);

        PrintLine(TEXT("Word is: %s"), *Results.KnownWord);
        FString ContainedLetters = "";
        for (TCHAR character : Results.KnownLetters) {
            ContainedLetters.AppendChar(character);
        }
        PrintLine(TEXT("Word contains: %s"), *ContainedLetters);

        FString NotContainedLetters = "";
        for (TCHAR character : Results.WrongLetters) {
            NotContainedLetters.AppendChar(character);
        }
        PrintLine(TEXT("Word does not have: %s"), *NotContainedLetters);

        PrintLine(TEXT("You have %i attempts left."), NumLives);
    }

}

bool UBullCowCartridge::IsIsogram(const FString& Word) {
    TSet<TCHAR> HashSet;
    FString LowerCaseWord = Word.ToLower();
    for (int i=0; i<LowerCaseWord.Len(); i++) {
        if (!HashSet.Contains(LowerCaseWord[i])) {
            HashSet.Add(LowerCaseWord[i]);
        } else {
            return false;
        }
    }

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& Words) const {
    TArray<FString> ValidWords;

    for (FString Word : Words) {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word)) {
            ValidWords.Emplace(Word);
        }
    }

    return ValidWords;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, TResults& results) {

    for (int32 i = 0; i < Guess.Len(); i++) {
        int32 index;
        if(HiddenWord.FindChar(Guess[i], index)) {
            if (index == i) {
                results.KnownWord[i] = Guess[i];
                results.KnownLetters.Remove(Guess[i]);
            } else {
                results.KnownLetters.Emplace(Guess[i]);
            }
        } else {
            results.WrongLetters.Emplace(Guess[i]);
        }
    }
}