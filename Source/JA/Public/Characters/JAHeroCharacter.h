// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Characters/JABaseCharacter.h"
#include "GameplayTagContainer.h"
#include "JAHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UDataAsset_InputConfig;
struct FInputActionValue;
class UHeroCombatComponent;
class UHeroUIComponent;
class UJACustomMovementComponent;
class UJAEquipmentComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityFailedBP, const UGameplayAbility*, Ability, const FGameplayTagContainer&, ReasonTags);

/**
 * 
 */
UCLASS()
class JA_API AJAHeroCharacter : public AJABaseCharacter
{
	GENERATED_BODY()
	
public:
	AJAHeroCharacter(const FObjectInitializer& ObjectInitializer);

public:
	//~ Begin IPawnCombatInterface Interface.
	virtual UPawnCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface.
	virtual UPawnUIComponent* GetPawnUIComponent() const override;
	virtual UHeroUIComponent* GetHeroUIComponent() const override;
	//~ End IPawnUIInterface Interface

	UPROPERTY(BlueprintAssignable, Category = "GAS|Events")
	FOnAbilityFailedBP OnAbilityFailedDispatcher;

public:
	FORCEINLINE UHeroCombatComponent* GetHeroCombatComponent() const { return HeroCombatComponent; }
	FORCEINLINE UJACustomMovementComponent* GetJACustomMovementComponent() const { return JACustomMovementComponent; }
	FORCEINLINE UJAEquipmentComponent* GetJAEquipmentComponent() const { return JAEquipmentComponent; }

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	virtual void PostInitializeComponents() override;
	//~ End APawn Interface

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
#pragma region Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UHeroCombatComponent* HeroCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UHeroUIComponent* HeroUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	UJACustomMovementComponent* JACustomMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	UJAEquipmentComponent* JAEquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ChestMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* PantsMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* GlovesMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* BootsMeshComp;

#pragma endregion

#pragma region Inputs

	void OnPlayerEnterClimbState();
	void OnPlayerExitClimbState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	UDataAsset_InputConfig* InputConfigDataAsset;

	UPROPERTY()
	FVector2D SwitchDirection = FVector2D::ZeroVector;

	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Climb_Action(const FInputActionValue& InputActionValue);

	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);	

	void Input_PickUpStarted(const FInputActionValue& InputActionValue);

	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);

#pragma endregion

private:
	void HandleGroundMovementInput(const FInputActionValue& InputActionValue);
	void HandleClimbMovementInput(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void HandleAbilityFailed(const UGameplayAbility* Ability, const FGameplayTagContainer& ReasonTags);

public:
	FORCEINLINE float GetCapsuleHeight() const { return CapsuleHeight; }

private:
	UPROPERTY()
	float CapsuleHeight = 96.f;
};
