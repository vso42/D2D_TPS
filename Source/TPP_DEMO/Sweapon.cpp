// Fill out your copyright notice in the Description page of Project Settings.


#include "Sweapon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ShapeComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"


static int32 debugdrawing = 0;
FAutoConsoleVariableRef Cvardebugdraw
(TEXT("TPS.DebugWeapons"),
	debugdrawing,
	TEXT("DRAW LINES FOR WEAPONS"),
	ECVF_Cheat);

// Sets default values
ASweapon::ASweapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	MuzzleSocketName = "MuzzleSocket";
	TracerName = "Target";

}


void ASweapon::Fire()
{
	AActor* myOwner = GetOwner();
	if (myOwner)
	{
		FVector eyeLoc;
		FRotator eyeRotation;
		myOwner->GetActorEyesViewPoint(eyeLoc, eyeRotation);
		FVector shotD = eyeRotation.Vector();
		FVector traceEndpt = eyeLoc + (shotD);
		FCollisionQueryParams queries; //for ignoring 
		queries.AddIgnoredActor(myOwner);
		queries.AddIgnoredActor(this);
		queries.bTraceComplex = true;
	     particlend = traceEndpt;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, eyeLoc, traceEndpt, ECC_Visibility, queries)) //traced
		{
			AActor* HitActor = Hit.GetActor();
			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, shotD, Hit, myOwner->GetInstigatorController(), this, DamageType);
			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
			particlend = Hit.ImpactPoint;
		}
		if (debugdrawing > 0)
		{
			DrawDebugLine(GetWorld(), eyeLoc, traceEndpt, FColor::White, false, 1.0f, 0, 1.0f);
		}
		PlayFireEffects(particlend);

	}


}

void ASweapon::PlayFireEffects(FVector traceover)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}
	if (TracerEffect)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter(TracerName, traceover);

		}
	}
	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner)
	{
		APlayerController* pc = Cast<APlayerController>(MyOwner->GetController());
		if (pc)
		{
			pc->ClientPlayCameraShake(FireCanShake);
		}
	}
}

