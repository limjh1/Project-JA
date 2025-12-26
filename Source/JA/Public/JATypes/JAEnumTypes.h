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