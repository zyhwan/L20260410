// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "MyStaticMeshComponent.h"
#include "MyRocket.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(40.0f, 44.0f, 12.0f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT
	("/Script/Engine.StaticMesh'/Game/P38/Body/SM_P38_Body.SM_P38_Body'"));

	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}


	Left = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateDefaultSubobject<UMyStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT
	("/Script/Engine.StaticMesh'/Game/P38/Propeller/SM_P38_Propeller.SM_P38_Propeller'"));

	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	Left->SetRelativeLocation(FVector(37.f,-21.f, 0));
	Right->SetRelativeLocation(FVector(37.f, 21.f, 0));

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(200.f, 0.f, 0));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->SocketOffset = FVector(0.f, 0.f, 33.33f);
	SpringArm->TargetArmLength = 120.f;
	SpringArm->bEnableCameraLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = MoveSpeed;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), BoostValue);

	//RotatePropeller(Left, PropellerRotationSpeed);
	//RotatePropeller(Right, PropellerRotationSpeed);
}

void AMyPawn::RotatePropeller(USceneComponent* Where, float Rotationspeed)
{
	Where->AddLocalRotation(FRotator(0.f, 0.f, Rotationspeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Input에 함수 등록(함수 포인터) 조건이 되면 자동으로 호출하는 함수 -> 이벤트
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Pressed, this, &AMyPawn::Boost);
	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Released, this, &AMyPawn::UnBoost);
}

void AMyPawn::Pitch(float value)
{
	AddActorLocalRotation(FRotator(RotationSpeed * value* UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), 0.f, 0.f));
}

void AMyPawn::Roll(float value)
{
	AddActorLocalRotation(FRotator(0.f, 0.f, RotationSpeed * value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AMyRocket>(AMyRocket::StaticClass(),
		Arrow->K2_GetComponentToWorld());
	

}

void AMyPawn::Boost()
{
	BoostValue = 1.f;
}

void AMyPawn::UnBoost()
{
	BoostValue = 0.5f;
}
