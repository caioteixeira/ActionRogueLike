// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCharacter.h"

#include "CTActionComponent.h"
#include "CTAttributeComponent.h"
#include "CTInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

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
	ActionComponent = CreateDefaultSubobject<UCTActionComponent>("ActionComponent");

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

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACTCharacter::SprintStart);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACTCharacter::SprintStop);	
	
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

void ACTCharacter::SprintStart()
{
	ActionComponent->StartActionByName(this, "Sprint");
}

void ACTCharacter::SprintStop()
{
	ActionComponent->StopActionByName(this, "Sprint");
}

void ACTCharacter::PrimaryAttack()
{
	ActionComponent->StartActionByName(this, "PrimaryAttack");
}

void ACTCharacter::BlackHoleAttack()
{
	ActionComponent->StartActionByName(this, "BlackHole");
}

void ACTCharacter::Dash()
{
	ActionComponent->StartActionByName(this, "Dash");

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

