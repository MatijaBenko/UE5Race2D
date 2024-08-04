#include "MyGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void AMyGameModeBase::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(ResetGameTimer, this, &AMyGameModeBase::UpdateTimer, 1.0f, true);
    if (UGameplayStatics::GetCurrentLevelName(GetWorld(), true) == "LevelOne")
    {
        ElaspedTime = LevelOneTime;
    }
}

void AMyGameModeBase::RestartLevel(bool IsWin)
{
    if (IsWin && ElaspedTime >= 0)
    {
        WinTextDelegate.Broadcast(ESlateVisibility::Visible);
        GetWorldTimerManager().SetTimer(ResetGameTimer, FTimerDelegate::CreateLambda([this, IsWin]()
            { OnDelayCompleted(IsWin); }), WinDelayTime, false);
    } else {
        TimeChangedDelegate.Broadcast(LevelOneTime);
        LostTextDelegate.Broadcast(ESlateVisibility::Visible);
        GetWorldTimerManager().SetTimer(ResetGameTimer, FTimerDelegate::CreateLambda([this, IsWin]()
            { OnDelayCompleted(IsWin); }), LoseDelayTime, false);
    }
}

void AMyGameModeBase::OnResetGameTimerTimeout()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName("LevelOne"));
}

void AMyGameModeBase::UpdateTimer()
{
    SetTime(1.0f);
}

void AMyGameModeBase::OnDelayCompleted(bool IsWin)
{
    if (IsWin)
    {
        APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (PlayerController)
        {
            UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
        }
    }
    else
    {
        LostTextDelegate.Broadcast(ESlateVisibility::Hidden);
        OnResetGameTimerTimeout();
    }
}

void AMyGameModeBase::SetTime(float NewTime)
{
    ElaspedTime -= NewTime;
    TimeChangedDelegate.Broadcast(ElaspedTime);
}