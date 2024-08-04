#include "Obstacles.h"
#include "PlayerChar.h"
#include "Kismet/GameplayStatics.h"

AObstacles::AObstacles()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	ObstacleSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpriteComp"));
	ObstacleSprite->SetupAttachment(RootComponent);
}

void AObstacles::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &AObstacles::OverlapBegin);
	CapsuleComp->OnComponentEndOverlap.AddDynamic(this, &AObstacles::OverlapEnd);

	AGameModeBase *Gamemode = UGameplayStatics::GetGameMode(GetWorld());
	MyGameMode = Cast<AMyGameModeBase>(Gamemode);
}

void AObstacles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AObstacles::OverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
							  int32 OtherBodyIndex, bool FromSweep, const FHitResult &SweepResult)
{
	APlayerChar *Player = Cast<APlayerChar>(OtherActor);
	if(Player)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
		if (IsMud)
		{
			Player->MovementSpeed = 450.f;
		} else {
			Player->CanMove = false;
			MyGameMode->RestartLevel(IsFinishLine);
		}
	}
}

void AObstacles::OverlapEnd(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
							int32 OtherBodyIndex)
{
	APlayerChar *Player = Cast<APlayerChar>(OtherActor);
	if(Player)
	{
		Player->MovementSpeed = 1000.f;
	}
}
