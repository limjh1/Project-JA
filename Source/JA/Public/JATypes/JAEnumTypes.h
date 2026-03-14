#pragma once

UENUM()
enum class EJAConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EJAValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EJASuccessType : uint8
{
	Successful,
	Failed
};

UENUM()
enum class EJACountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EJACountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class EJAGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class EJAInputMode : uint8
{
	GameOnly,
	UIOnly
};

UENUM(BlueprintType)
enum class EIMCPriority : uint8
{
	Default,
	Weapon,
	Climb,
	LockOn
};

UENUM(BlueprintType)
namespace ECustomMovementMode
{
	enum Type
	{
		MOVE_Climb UMETA(DisplayName = "Climb Mode")
	};
}

UENUM(BlueprintType)
enum class EHopType : uint8
{
	HopUp,
	HopDown,
	HopLeft,
	HopRight,
	HopUpLeft,
	HopUpRight,
	Max, // Invalid
};

UENUM(BlueprintType)
enum class EConfirmScreenType : uint8
{
	Ok,
	YesNo,
	OkCancel,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EOptionListDataModifyReason :uint8
{
	DirectlyModified,
	DependencyModified,
	ResetToDefault
};

UENUM(BlueprintType)
enum class EJAEquipmentType :uint8
{
	Armour_Chest,
	Armour_Pants,
	Armour_Boots,
	Armour_Gloves
};