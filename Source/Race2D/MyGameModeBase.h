#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Components/SlateWrapperTypes.h"
#include "MyGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTimeChangedDelegate, float, NewTimer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLostTextDelegate, ESlateVisibility, IsVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWinTextDelegate, ESlateVisibility , IsVisible);
UCLASS()
class RACE2D_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoseDelayTime = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WinDelayTime = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LevelOneTime = 100.f;

	UPROPERTY(BlueprintAssignable)
	FTimeChangedDelegate TimeChangedDelegate;

	UPROPERTY(BlueprintAssignable)
	FLostTextDelegate LostTextDelegate;

	UPROPERTY(BlueprintAssignable)
	FWinTextDelegate WinTextDelegate;

	void RestartLevel(bool IsWin);
	void OnResetGameTimerTimeout();
	void UpdateTimer();
	void OnDelayCompleted(bool IsWin);
private:
	FTimerHandle ResetGameTimer;

	void SetTime(float NewTime);

	float ElaspedTime = 0.f;
	
};
