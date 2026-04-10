// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRocket.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
AMyRocket::AMyRocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(29.470973,8.587499,9.328462));

	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Rocket->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT
	("/Script/Engine.StaticMesh'/Game/P38/Rocket/SM_Rocket.SM_Rocket'"));

	if (SM_Body.Succeeded())
	{
		Rocket->SetStaticMesh(SM_Body.Object);
	}

	Rocket->SetRelativeRotation(FRotator(-90.f, 0, 0));

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 1000.f;
	Movement->MaxSpeed = 1000.f;
	Movement->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void AMyRocket::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorBeginOverlap.AddDynamic(this, &AMyRocket::ProcessActorBeginOverlap);

	SetLifeSpan(3.0f);

}

// Called every frame
void AMyRocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyRocket::ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Destroy!!"));
	Destroy();
}

