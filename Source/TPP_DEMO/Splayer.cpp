// Fill out your copyright notice in the Description page of Project Settings.
#include "Splayer.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Sweapon.h"


// Sets default values
ASplayer::ASplayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent); 
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;


	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	zoomedFOV = 65.0f;
	ZoomSpeed = 20.0f;
	WeaponAttachSocketName = "WeaponSocket";
	
}

// Called when the game starts or when spawned
void ASplayer::BeginPlay()
{
	Super::BeginPlay();
	defaultFOV= CameraComp->FieldOfView;
	FActorSpawnParameters spawnpar;
	spawnpar.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	 weapon = GetWorld()->SpawnActor<ASweapon>(curstarterweapon, FVector::ZeroVector, FRotator::ZeroRotator,spawnpar);
	 if (weapon)
	 {
		 weapon->SetOwner(this);
		 weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
	 }
	
}
void ASplayer::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);

}
void ASplayer::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}
void ASplayer::BeginCrouch()
{
	Crouch();
}
void ASplayer::EndCrouch()
{
	UnCrouch();
}
void ASplayer::JumpIn()
{
	Jump();
}
void ASplayer::BeginZoom()
{
	bwantstozoom = true;
}
void ASplayer::EndZoom()
{
	bwantstozoom = false;
}
void ASplayer::Fire()
{
	if (weapon)
	{
		weapon->Fire();
	}
}
// Called every frame
void ASplayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float targetFOV = bwantstozoom ? zoomedFOV : defaultFOV;
	float currentFOV = FMath::FInterpTo(CameraComp->FieldOfView, targetFOV, DeltaTime, ZoomSpeed);

	CameraComp->SetFieldOfView(currentFOV);
     
}

// Called to bind functionality to input
void ASplayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASplayer :: MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASplayer :: MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, & ASplayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASplayer::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASplayer::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASplayer::EndCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASplayer::Jump);

	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASplayer::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASplayer::EndZoom);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASplayer::Fire);

}

FVector ASplayer::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

