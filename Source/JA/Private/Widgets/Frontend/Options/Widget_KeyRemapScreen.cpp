// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/Widget_KeyRemapScreen.h"
#include "CommonRichTextBlock.h"
#include "Framework/Application/IInputProcessor.h"
#include "CommonInputSubsystem.h"
#include "ICommonInputModule.h"
#include "CommonUITypes.h"

#include "JADebugHelper.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListenTo, ULocalPlayer* InOwningLocalPlayer)
		: CachedInputTypeToListenTo(InInputTypeToListenTo),
		CachedWeakOwningLocalPlayer(InOwningLocalPlayer)
	{
	}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDeleagte, const FKey& /*PressedKey*/)
	FOnInputPreProcessorKeyPressedDeleagte OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectCanceledDelegate, const FString& /*CanceledReason*/);
	FOnInputPreProcessorKeySelectCanceledDelegate OnInputPreprocessorSelectCanceled;

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{

	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());

		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());

		return true;
	}

	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreprocessorSelectCanceled.ExecuteIfBound(TEXT("키 설정이 취소되었습니다"));
			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedWeakOwningLocalPlayer.Get());

		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (InPressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreprocessorSelectCanceled.ExecuteIfBound(TEXT("키 설정 중 게임패드 입력이 감지되어 취소되었습니다."));
				return;
			}

			break;
		case ECommonInputType::Gamepad:
			if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey == EKeys::LeftMouseButton)
			{
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));

				check(InputActionData);
				
				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());

				return;
			}

			if (!InPressedKey.IsGamepadKey())
			{
				OnInputPreprocessorSelectCanceled.ExecuteIfBound(TEXT("키 설정 중 게임패드가 아닌 입력이 감지되어 취소되었습니다."));
				return;
			}

			break;
		default:
			break;
		}

		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressedKey);
	}

private:
	ECommonInputType CachedInputTypeToListenTo;
	
	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
};

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(CachedDesiredInputType, GetOwningLocalPlayer());
	CachedInputPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	CachedInputPreprocessor->OnInputPreprocessorSelectCanceled.BindUObject(this, &ThisClass::OnKeySelectCanceled);

	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);

	FString InputDeviceName;

	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("키보드 / 마우스");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("게임 패드");
		break;
	default:
		break;
	}

	const FString DisplayRichMsg = FString::Printf(
		TEXT("<KeyRemapDefault>아무</> <KeyRemapHighlight>%s</> <KeyRemapDefault>키를 누르세요.</>"), *InputDeviceName
	);

	CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMsg));
}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);

		CachedInputPreprocessor.Reset();
	}
}

void UWidget_KeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget(
		[this, PressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UWidget_KeyRemapScreen::OnKeySelectCanceled(const FString& CanceledReason)
{
	RequestDeactivateWidget(
		[this, CanceledReason]()
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	// Delay a tick to make sure the input is processed correctly
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda(
			[PreDeactivateCallback, this](float DeltaTime)->bool
			{
				PreDeactivateCallback();

				DeactivateWidget();

				return false;
			}
		)
	);
}
