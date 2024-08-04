#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "PaperSpriteComponent.h"
#include "MyGameModeBase.h"
#include "Sound/SoundBase.h"
#include "Obstacles.generated.h"

UCLASS()
class RACE2D_API AObstacles : public AActor
{
	GENERATED_BODY()


protected:
	virtual void BeginPlay() override;

public:	
	AObstacles();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent *CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPaperSpriteComponent *ObstacleSprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase *HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsMud = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsFinishLine = false;

	AMyGameModeBase *MyGameMode;

	UFUNCTION()
	void OverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult);

	UFUNCTION()
	void OverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
	int32 OtherBodyIndex);

};
