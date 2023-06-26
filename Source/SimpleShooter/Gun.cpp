// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
	{
		return nullptr;
	}
	return OwnerPawn->GetController();
}

bool AGun::GunTraceFunction(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr)
	{
		return false;
	}
	FVector Location;
	FRotator Rotation;

	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1,Params);
}

void AGun::PullTrigger()
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh,TEXT("MuzzleFlashSocket"));
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),MuzzleSound, Mesh->GetSocketLocation("MuzzleFlashSocket"),0.2);
	
	FHitResult HitResult;
	FVector ShotDirection;
	bool bSuccess = GunTraceFunction(HitResult,ShotDirection);
	
	
	if(bSuccess)
	{
		
		if(HitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitResult.Location, ShotDirection.Rotation());
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitResult.Location,0.1);
			
			AActor* ShotActor = HitResult.GetActor();
			if(ShotActor != nullptr)
			{
				FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
				AController* OwnerController = GetOwnerController();
				ShotActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			}
		}
	}
	
}


