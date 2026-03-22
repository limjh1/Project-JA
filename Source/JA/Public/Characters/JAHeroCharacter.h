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
class UPotionInventoryComponent;
class UAfterImageComponent;

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
	FORCEINLINE UPotionInventoryComponent* GetPotionInventoryComponent() const { return PotionInventoryComponent; }
	FORCEINLINE UAfterImageComponent* GetAfterImageComponent() const { return AfterImageComponent; }

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
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHeroCombatComponent> HeroCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHeroUIComponent> HeroUIComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UJACustomMovementComponent> JACustomMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UJAEquipmentComponent> JAEquipmentComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> ChestMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> PantsMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> GlovesMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment | Armour", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> BootsMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Potion", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPotionInventoryComponent> PotionInventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat | After Image", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAfterImageComponent> AfterImageComponent;
#pragma endregion

#pragma region Inputs

	void OnPlayerEnterClimbState();
	void OnPlayerExitClimbState();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CharacterData", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataAsset_InputConfig> InputConfigDataAsset;

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
