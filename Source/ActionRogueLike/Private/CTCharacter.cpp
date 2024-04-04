// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCharacter.h"

#include "CTAttributeComponent.h"
#include "CTInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "CTAttributeComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACTCharacter::ACTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	InteractionComponent = CreateDefaultSubobject<UCTInteractionComponent>("InteractionComponent");

	AttributeComponent = CreateDefaultSubobject<UCTAttributeComponent>("AttributeComponent");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}

void ACTCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ACTCharacter::OnHealthChanged);
}

// Called when the game starts or when spawned
void ACTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ACTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ACTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACTCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ACTCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ACTCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("Dash", IE_Pressed, this, &ACTCharacter::Dash);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACTCharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ACTCharacter::PrimaryInteract);
}

void ACTCharacter::HealSelf(float Amount)
{
	AttributeComponent->ApplyHealthChange(this, Amount);
}

void ACTCharacter::OnHealthChanged(AActor* InstigatorActor, UCTAttributeComponent* OwningComponent, float newHealth,
                                   float Delta)
{
	GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
	
	if (newHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ACTCharacter::MoveForward(float value)
{
	FRotator controlRotator = GetControlRotation();
	controlRotator.Pitch = 0.0f;
	controlRotator.Roll = 0.0f;
	AddMovementInput(controlRotator.Vector(), value);
}

void ACTCharacter::MoveRight(float value)
{
	FRotator controlRotator = GetControlRotation();
	controlRotator.Pitch = 0.0f;
	controlRotator.Roll = 0.0f;

	const auto right = FRotationMatrix(controlRotator).GetScaledAxis(EAxis::Y);
	AddMovementInput(right, value);
}

void ACTCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this,
		&ACTCharacter::PrimaryAttack_TimerElapsed, 0.2f);
}

void ACTCharacter::SpawnProjectile(TSubclassOf<AActor> ClassToSpawn)
{
	if (ensureAlways(ClassToSpawn))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;

		FHitResult Hit;
		FVector TraceStart = CameraComponent->GetComponentLocation();
		FVector TraceEnd = CameraComponent->GetComponentLocation() + (GetControlRotation().Vector() * 5000);

		FCollisionShape Shape;
		float Radius = 20.0f;
		Shape.SetSphere(Radius);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FRotator ProjRotation;
		if (GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape, Params))
		{
			ProjRotation = FRotationMatrix::MakeFromX(Hit.ImpactPoint - HandLocation).Rotator();
		}
		else
		{
			ProjRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		}
		
		const FTransform SpawnTM = FTransform(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ClassToSpawn, SpawnTM, SpawnParams);
	}
}

void ACTCharacter::PrimaryAttack_TimerElapsed()
{
	SpawnProjectile(ProjectileClass);
}

void ACTCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_BlackHoleAttack, this,
			&ACTCharacter::BlackHoleAttack_TimerElapsed, 0.2f);
}

void ACTCharacter::BlackHoleAttack_TimerElapsed()
{
	SpawnProjectile(BlackHoleProjectileClass);
}

void ACTCharacter::Dash()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_Dash, this,
			&ACTCharacter::Dash_TimerElapsed, 0.2f);
}

void ACTCharacter::Dash_TimerElapsed()
{
	SpawnProjectile(DashProjectileClass);
}

void ACTCharacter::PrimaryInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

FVector ACTCharacter::GetPawnViewLocation() const
{
	return CameraComponent->GetComponentLocation();
}

