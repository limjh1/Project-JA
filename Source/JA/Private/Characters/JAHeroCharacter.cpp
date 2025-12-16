// JhLim All Rights Reserved


#include "Characters/JAHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/Input/DataAsset_InputConfig.h"
#include "Components/Input/JAInputComponent.h"
#include "AbilitySystem/JAAbilitySystemComponent.h"
#include "JAGameplayTags.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "DataAssets/StartUpData/DataAsset_HeroStartUpData.h"
#include "Components/UI/HeroUIComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "JADebugHelper.h"

AJAHeroCharacter::AJAHeroCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	HeroCombatComponent = CreateDefaultSubobject<UHeroCombatComponent>(TEXT("HeroCombatComponent"));
	HeroUIComponent = CreateDefaultSubobject<UHeroUIComponent>(TEXT("HeroUIComponent"));
}

UPawnCombatComponent* AJAHeroCharacter::GetPawnCombatComponent() const
{
	return HeroCombatComponent;
}

UPawnUIComponent* AJAHeroCharacter::GetPawnUIComponent() const
{
	return HeroUIComponent;
}

UHeroUIComponent* AJAHeroCharacter::GetHeroUIComponent() const
{
	return HeroUIComponent;
}

void AJAHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!CharacterStartUpData.IsNull())
	{
		if (UDataAsset_StartUpDataBase* LoadedData = CharacterStartUpData.LoadSynchronous())
		{
			LoadedData->GiveToAbilitySystemComponent(JAAbilitySystemComponent);
		}		
	}
}

void AJAHeroCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AJAHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as input config"));

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(SubSystem);

	SubSystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);

	UJAInputComponent* JAInputComponent = CastChecked<UJAInputComponent>(PlayerInputComponent);

	JAInputComponent->BindNativeInputAction(InputConfigDataAsset, JAGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	JAInputComponent->BindNativeInputAction(InputConfigDataAsset, JAGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);

	JAInputComponent->BindNativeInputAction(InputConfigDataAsset, JAGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	JAInputComponent->BindNativeInputAction(InputConfigDataAsset, JAGameplayTags::InputTag_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	JAInputComponent->BindAbilityInputAction(InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed, &ThisClass::Input_AbilityInputReleased);
}

void AJAHeroCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if (0.f != MovementVector.Y)
	{
		const FVector ForwardDir = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDir, MovementVector.Y);
	}

	if (0.f != MovementVector.X)
	{
		const FVector RightDir = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDir, MovementVector.X);
	}
}

void AJAHeroCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (0.f != LookAxisVector.X)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if (0.f != LookAxisVector.Y)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AJAHeroCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	SwitchDirection = InputActionValue.Get<FVector2D>();
}

void AJAHeroCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		(0.f < SwitchDirection.X) ? JAGameplayTags::Player_Event_SwitchTarget_Right : JAGameplayTags::Player_Event_SwitchTarget_Left,
		Data
	);

}

void AJAHeroCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	JAAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void AJAHeroCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	JAAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}
