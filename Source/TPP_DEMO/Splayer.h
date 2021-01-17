// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "Splayer.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASweapon;

UCLASS()
class TPP_DEMO_API ASplayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASplayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void MoveForward(float Value);
	void MoveRight(float Value);
	void BeginCrouch();
	void EndCrouch();
	void JumpIn();
	void BeginZoom();
	void EndZoom();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	bool bwantstozoom;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float zoomedFOV;
	
	float defaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Player" , meta=(ClampMin=0.1,ClmapMax=100.0))
	float ZoomSpeed;
	ASweapon* weapon;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	TSubclassOf<ASweapon> curstarterweapon;
	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	FName WeaponAttachSocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual FVector GetPawnViewLocation() const override;
	UFUNCTION(BlueprintCallable,Category="Weapon")
	void Fire();


};
