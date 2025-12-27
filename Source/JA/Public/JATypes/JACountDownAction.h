// JhLim All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "JATypes/JAEnumTypes.h"

class FJACountDownAction : public FPendingLatentAction
{
public:
	FJACountDownAction(float InTotalCountDownTime, float InUpdateInterval, float& InOutRemainingTime, EJACountDownActionOutput& InCountDownOutput, const FLatentActionInfo& LatentInfo)
		: bNeedToCancel(false), TotalCountDownTime(InTotalCountDownTime), UpdateInterval(InUpdateInterval), OutRemainingTime(InOutRemainingTime),
		CountDownOutput(InCountDownOutput), ExecutionFunction(LatentInfo.ExecutionFunction), OutputLink(LatentInfo.Linkage),
		CallbackTarget(LatentInfo.CallbackTarget), ElapsedInterval(0.f), ElapsedTimeSinceStart(0.f)
	{

	}

public:
	virtual void UpdateOperation(FLatentResponse& Response) override;

	void CancelAction();

private:
	bool bNeedToCancel;
	float TotalCountDownTime;
	float UpdateInterval;
	float& OutRemainingTime;
	EJACountDownActionOutput& CountDownOutput;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	float ElapsedInterval;
	float ElapsedTimeSinceStart;
};