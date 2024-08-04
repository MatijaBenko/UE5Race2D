#include "PlayerChar.h"

APlayerChar::APlayerChar()
{

	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	Camera->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	CarSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Car Sprite"));
	CarSpriteComp->SetupAttachment(RootComponent);

	BaseAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BaseAudioComp"));
	BaseAudioComponent->SetupAttachment(RootComponent);

	AccelerateAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AccelerateAudioComp"));
	AccelerateAudioComponent->SetupAttachment(RootComponent);
	AccelerateAudioComponent->bAutoActivate = false;
}

void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	APlayerController *PlayerController = Cast<APlayerController>(Controller);

	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem *SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(InputMappingContext, 0);
		}
		if (BaseCarSound)
		{
			BaseAudioComponent->SetSound(BaseCarSound);
			BaseAudioComponent->Play();
		}
		if (AccelerateCarSound)
		{
			AccelerateAudioComponent->SetSound(AccelerateCarSound);
		}
	}
}

void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsAccelerating && CurrentBoostValue < 100.f)
	{
		DecreaseProgressBar(DeltaTime);
	}
	else
	{
		RegenerateProgressBar(DeltaTime);
	}
}

void APlayerChar::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent *EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComp)
	{
		EnhancedInputComp->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerChar::Move);
		EnhancedInputComp->BindAction(AccelerateAction, ETriggerEvent::Triggered, this, &APlayerChar::Accelerate);
		EnhancedInputComp->BindAction(AccelerateAction, ETriggerEvent::Completed, this, &APlayerChar::StopAccelerate);
	}
}

void APlayerChar::Move(const FInputActionValue &Value)
{
	FVector2D MoveActionValue = Value.Get<FVector2D>();
	if (CanMove)
	{
		if (abs(MoveActionValue.Y) > 0.f)
		{
			float DeltaTime = GetWorld()->DeltaTimeSeconds;

			if (abs(MoveActionValue.X) > 0.f)
			{
				float RotationAmount = -RotationSpeed * MoveActionValue.X * DeltaTime;
				AddActorWorldRotation(FRotator(RotationAmount, 0.f, 0.f));
			}

			float FinalMovementSpeed = MovementSpeed;

			// Moving Backwards
			if (MoveActionValue.Y < 0.f)
			{
				FinalMovementSpeed *= 0.5;
			}

			FVector CurrentLocation = GetActorLocation();
			FVector DistanceToMove = GetActorUpVector() * FinalMovementSpeed * MoveActionValue.Y * DeltaTime;
			FVector NewLocation = CurrentLocation + DistanceToMove;
			SetActorLocation(NewLocation);
		}
	}
}

void APlayerChar::Accelerate(const FInputActionValue &Value)
{
	MovementSpeed = AccelerateSpeed;
	bIsAccelerating = true;
	if (!AccelerateAudioComponent->IsPlaying())
	{
		AccelerateAudioComponent->Play();
	}
}

void APlayerChar::StopAccelerate(const FInputActionValue &Value)
{
	MovementSpeed = 1000.f;
	bIsAccelerating = false;
	if (AccelerateAudioComponent->IsPlaying())
	{
		AccelerateAudioComponent->Stop();
	}
}

void APlayerChar::DecreaseProgressBar(float DeltaTime)
{
	if (CurrentBoostValue > 0)
	{
		CurrentBoostValue -= 1.f * DeltaTime;
		ProgressBarDelegate.Broadcast(-1.0f * DeltaTime);
	}
	else
	{
		StopAccelerate(FInputActionValue());
	}
}

void APlayerChar::RegenerateProgressBar(float DeltaTime)
{
	if (CurrentBoostValue < BoostProgressBarMax)
	{
		CurrentBoostValue += 0.2f * DeltaTime;
		ProgressBarDelegate.Broadcast(0.2f * DeltaTime);
	}
}