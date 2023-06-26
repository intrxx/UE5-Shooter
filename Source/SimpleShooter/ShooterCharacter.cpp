// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gun.h"
#include "SimpleShooterGameModeBase.h"
#include "Components/CapsuleComponent.h"
#include "Windows/LiveCoding/Private/External/LC_ClientStartupThread.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	ArmComp->SetupAttachment(ACharacter::RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(ArmComp);
	
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
	
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);

	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);

	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//mouse and keyboard
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookSide"),this,&APawn::AddControllerYawInput);

	//both m&k and gamepad
	PlayerInputComponent->BindAxis(TEXT("MoveSide"), this, &AShooterCharacter::MoveSides);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAction(TEXT("Jump"),EInputEvent::IE_Pressed,this,&ACharacter::Jump);

	//gamepad
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"), this, &AShooterCharacter::LookRightRate);

	//shooting
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Fire);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveSides(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
	AddControllerPitchInput(AxisValue * LookUpSpeed * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
	AddControllerYawInput(AxisValue * LookRightSpeed * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Fire()
{
	Gun->PullTrigger();
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	DamageApplied = FMath::Min(CurrentHealth, DamageApplied);
	CurrentHealth -= DamageApplied;

	if(IsDead())
	{
		ASimpleShooterGameModeBase* Gamemode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(Gamemode != nullptr)
		{
			Gamemode->PawnKilled(this);
		}
		
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageApplied;
}

bool AShooterCharacter::IsDead() const
{
	if(CurrentHealth <= 0)
	{
		return true;
	}
		return false;
}

float AShooterCharacter::GetHealthPercent() const
{
	return CurrentHealth / MaxHealth;
}





