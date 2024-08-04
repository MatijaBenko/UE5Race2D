#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperSpriteComponent.h"
#include "PlayerChar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProgressBarDelegate, float, NewBoost);
UCLASS()
class RACE2D_API APlayerChar : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:	
	APlayerChar();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Settings")
	UCapsuleComponent *CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Settings")
	USpringArmComponent *SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Settings")
	UCameraComponent *Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Settings")
	UPaperSpriteComponent *CarSpriteComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
	UInputMappingContext *InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
	UInputAction *MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Settings")
	UInputAction *AccelerateAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float MovementSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float AccelerateSpeed = 1450.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float RotationSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	USoundBase *BaseCarSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	USoundBase *AccelerateCarSound;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Settings")
	UAudioComponent *BaseAudioComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Settings")
	UAudioComponent *AccelerateAudioComponent;

	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FProgressBarDelegate ProgressBarDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Settings")
	float BoostProgressBarMax = 1.f;

	bool CanMove = true;

	void Move(const FInputActionValue &Value);
	void Accelerate(const FInputActionValue &Value);
	void StopAccelerate(const FInputActionValue &Value);

private:
	bool bIsAccelerating = false;
	float CurrentBoostValue = BoostProgressBarMax;
	void DecreaseProgressBar(float DeltaTime);
	void RegenerateProgressBar(float DeltaTime);
};
