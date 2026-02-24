// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Frontend/Options/OptionsDataInteractionHelper.h"

FCommonNumberFormattingOptions UListDataObject_Scalar::NoDecimal()
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = 0;
	return Options;
}

FCommonNumberFormattingOptions UListDataObject_Scalar::WithDecimal(int32 NumFracDigit)
{
	FCommonNumberFormattingOptions Options;
	Options.MaximumFractionalDigits = NumFracDigit;
	return Options;
}

float UListDataObject_Scalar::GetCurrentValue() const
{
	if (DataDynamicGetter)
	{
		return FMath::GetMappedRangeValueClamped(
			OutputValueRange,
			DisplayValueRange,
			StringToFloat(DataDynamicGetter->GetValueAsString())
		);
	}

	return 0.f;
}

void UListDataObject_Scalar::SetCurrentValueFromSlider(float InNewValue)
{
	if (DataDynamicSetter)
	{
		const float ClampedValue = FMath::GetMappedRangeValueClamped(
			DisplayValueRange,
			OutputValueRange,
			InNewValue
		);

		DataDynamicSetter->SetValueFromString(LexToString(ClampedValue));

		NotifyListDataModified(this);
	}
}

bool UListDataObject_Scalar::CanResetBackToDefaultValue() const
{
	if (HasDefaultValue() && DataDynamicGetter)
	{
		const float DefaultValue = StringToFloat(GetDefaultValueAsString());
		const float CurrentValue = StringToFloat(DataDynamicGetter->GetValueAsString());

		return !FMath::IsNearlyEqual(DefaultValue, CurrentValue, 0.01f);
	}

	return false;
}

bool UListDataObject_Scalar::TryResetBackToDefaultValue()
{
	if (CanResetBackToDefaultValue())
	{
		if (DataDynamicSetter)
		{
			DataDynamicSetter->SetValueFromString(GetDefaultValueAsString());

			NotifyListDataModified(this, EOptionListDataModifyReason::ResetToDefault);

			return true;
		}
	}

	return false;
}

void UListDataObject_Scalar::OnEditDependencyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionListDataModifyReason ModifyReason)
{
	NotifyListDataModified(this, EOptionListDataModifyReason::DependencyModified);

	Super::OnEditDependencyDataModified(ModifiedDependencyData, ModifyReason);
}

void UListDataObject_Scalar::NotifyListDataModified(UListDataObject_Base* ModifiedData, EOptionListDataModifyReason ModifyReason)
{
	//#TODO 추후, 저장해야할 객체 저장해서, 탭 옮길때 일괄저장되게 구조 수정할 것.
	Super::NotifyListDataModified(ModifiedData, ModifyReason);
}

float UListDataObject_Scalar::StringToFloat(const FString& InString) const
{
	float OutConvertedValue = 0.f;
	LexFromString(OutConvertedValue, *InString);
	return OutConvertedValue;
}
