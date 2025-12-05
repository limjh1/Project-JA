// JhLim All Rights Reserved


#include "AbilitySystem/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "AbilitySystem/JAAttributeSet.h"
#include "JAGameplayTags.h"

#include "JADebugHelper.h"

struct FJADamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken);

	FJADamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJAAttributeSet, AttackPower, Source, false /*#TODO 추후 버프/디버프 넣는다면 확인 필요*/);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJAAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UJAAttributeSet, DamageTaken, Target, false);
	}
};

static const FJADamageCapture& GetJADamageCapture()
{
	static FJADamageCapture JADamageCapture;
	return JADamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	// 캡쳐 느린, 효율성 낮은 작업 방법
	//FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
	//	UJAAttributeSet::StaticClass(),
	//	GET_MEMBER_NAME_CHECKED(UJAAttributeSet, AttackPower)
	//);

	//FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	//	AttackPowerProperty,
	//	EGameplayEffectAttributeCaptureSource::Source,
	//	false // #TODO 추후 버프/디버프 넣는다면 확인 필요
	//);

	//RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);

	RelevantAttributesToCapture.Add(GetJADamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetJADamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetJADamageCapture().DamageTakenDef);
}

void UGEExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	//EffectSpec.GetContext().GetSourceObject();
	//EffectSpec.GetContext().GetAbility();
	//EffectSpec.GetContext().GetInstigator();
	//EffectSpec.GetContext().GetEffectCauser();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetJADamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"), SourceAttackPower);

	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;
	
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(JAGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
			//Debug::Print(TEXT("BaseDamage"), BaseDamage);
		}

		if (TagMagnitude.Key.MatchesTagExact(JAGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedLightAttackComboCount"), UsedLightAttackComboCount);
		}

		if (TagMagnitude.Key.MatchesTagExact(JAGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
			//Debug::Print(TEXT("UsedHeavyAttackComboCount"), UsedHeavyAttackComboCount);
		}
	}

	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetJADamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	//Debug::Print(TEXT("TargetDefensePower"), TargetDefensePower);

	//#TODO 5%, 15% 데미지 콤보 보상치 수정 필요. 그리고 공격 종류에 상관없이 통합 콤보 인덱스로 UI 띄우기
	if (0 != UsedLightAttackComboCount)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05f + 1.f;

		BaseDamage *= DamageIncreasePercentLight;
		//Debug::Print(TEXT("ScaledBaseDamageLight"), BaseDamage);
	}

	if (0 != UsedHeavyAttackComboCount)
	{
		const float DamageIncreasePercentHeavy = UsedHeavyAttackComboCount * 0.15f + 1.f;

		BaseDamage *= DamageIncreasePercentHeavy;
		//Debug::Print(TEXT("ScaledBaseDamageHeavy"), BaseDamage);
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	//Debug::Print(TEXT("FinalDamageDone"), FinalDamageDone);

	if (0.f < FinalDamageDone)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetJADamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
			)
		);
	}
}
