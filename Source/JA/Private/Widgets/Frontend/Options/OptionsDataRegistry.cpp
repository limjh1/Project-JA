// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/OptionsDataRegistry.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Frontend/Options/OptionsDataInteractionHelper.h"
#include "JASettings/JAGameUserSettings.h"
#include "JAFunctionLibrary.h"
#include "JAGameplayTags.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Internationalization/StringTableRegistry.h"
#include "EnhancedInputSubsystems.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_KeyRemap.h"

#include "JADebugHelper.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UJAGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) LOCTABLE("/Game/UI/StringTable/ST_OptionsScreenDescription.ST_OptionsScreenDescription", InKey)

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwiningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab(InOwiningLocalPlayer);
	InitSystemCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	TObjectPtr<UListDataObject_Collection> const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildListItems.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}

	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Tab_Gameplay"));

	// This is the full code for contructor data interactor helper
	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper =
		MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UJAGameUserSettings, GetCurrentGameDifficulty));*/

	// Game Difficulty
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_GameDifficulty"));
		GameDifficulty->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_GameDifficulty"));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), GET_DESCRIPTION("UI_Options_Value_Difficulty_Easy"));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), GET_DESCRIPTION("UI_Options_Value_Difficulty_Normal"));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), GET_DESCRIPTION("UI_Options_Value_Difficulty_Hard"));
		GameDifficulty->AddDynamicOption(TEXT("Very Hard"), GET_DESCRIPTION("UI_Options_Value_Difficulty_VeryHard"));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplySettingsImmediatly(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Tab_Audio"));

	// Volume Category
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Category_Volume"));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_OverallVolume"));
			OverallVolume->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_OverallVolume"));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // No Decimal: 50% // One Decimal: 50.5%
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplySettingsImmediatly(true);

			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		// BGM Volume
		{
			UListDataObject_Scalar* BGMVolume = NewObject<UListDataObject_Scalar>();
			BGMVolume->SetDataID(FName("BGMVolume"));
			BGMVolume->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_BGMVolume"));
			BGMVolume->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_BGMVolume"));
			BGMVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			BGMVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			BGMVolume->SetSliderStepSize(0.01f);
			BGMVolume->SetDefaultValueFromString(LexToString(1.f));
			BGMVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			BGMVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // No Decimal: 50% // One Decimal: 50.5%
			BGMVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetBGMVolume));
			BGMVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetBGMVolume));
			BGMVolume->SetShouldApplySettingsImmediatly(true);

			VolumeCategoryCollection->AddChildListData(BGMVolume);
		}

		// SoundFX Volume
		{
			UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_SoundFXVolume"));
			SoundFXVolume->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_SoundFXVolume"));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal()); // No Decimal: 50% // One Decimal: 50.5%
			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			SoundFXVolume->SetShouldApplySettingsImmediatly(true);

			VolumeCategoryCollection->AddChildListData(SoundFXVolume);
		}
	}

	// Sound Category
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Category_Sound"));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		// Allow Background Audio
		{
			UListDataObject_StringBool* AllowBackgrounAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgrounAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgrounAudio->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_AllowBackgroundAudio"));
			AllowBackgrounAudio->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_AllowBackgroundAudio"));
			AllowBackgrounAudio->OverrideTrueDisplayText(GET_DESCRIPTION("UI_Options_Value_Enable"));
			AllowBackgrounAudio->OverrideFalseDisplayText(GET_DESCRIPTION("UI_Options_Value_Disable"));
			AllowBackgrounAudio->SetFalseAsDefaultValue();
			AllowBackgrounAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgrounAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgrounAudio->SetShouldApplySettingsImmediatly(true);

			SoundCategoryCollection->AddChildListData(AllowBackgrounAudio);
		}

		// Use HDR Audio
		{
			UListDataObject_StringBool* UseHDRAudioMode = NewObject<UListDataObject_StringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_UseHDRAudioMode"));
			UseHDRAudioMode->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_UseHDRAudioMode"));
			UseHDRAudioMode->OverrideTrueDisplayText(GET_DESCRIPTION("UI_Options_Value_Enable"));
			UseHDRAudioMode->OverrideFalseDisplayText(GET_DESCRIPTION("UI_Options_Value_Disable"));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplySettingsImmediatly(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Tab_Video"));

	UListDataObject_StringEnum* CreatedWindowMode = nullptr;

	// Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Category_Display"));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool
			{
				const bool bIsInEditor = (GIsEditor || GIsPlayInEditorWorld);

				return !bIsInEditor;
			}
		);
		PackagedBuildOnlyCondition.SetDisabledRichReason(GET_DESCRIPTION("UI_Options_Warning_PackagedBuildOnly").ToString());

		// Window Mode
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_WindowMode"));
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_WindowMode"));// FText::FromString(TEXT("전체 화면, 창 모드 등 표시 형태를 설정합니다.")));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, GET_DESCRIPTION("UI_Options_Value_WindowMode_Fullscreen"));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, GET_DESCRIPTION("UI_Options_Value_WindowMode_WindowedFullscreen"));
			WindowMode->AddEnumOption(EWindowMode::Windowed, GET_DESCRIPTION("UI_Options_Value_WindowMode_Windowed"));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediatly(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		// Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_ScreenResolution"));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_ScreenResolutions"));// FText::FromString(TEXT("화면에 출력되는 픽셀 해상도를 설정합니다.")));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediatly(true);

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					const bool bIsBoderlessWindow = CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;

					return !bIsBoderlessWindow;
				}
			);
			WindowModeEditCondition.SetDisabledRichReason(GET_DESCRIPTION("UI_Options_Warning_ResolutionBorderless").ToString());
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(WindowModeEditCondition);

			ScreenResolution->AddEditDependencyData(CreatedWindowMode);

			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	// Graphics Category
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Category_Graphics"));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		// Display Gamma
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_DisplayGamma"));
			DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_DisplayGamma"));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); // The Default value unreal has is: 2.2f
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UListDataObject_StringInteger* CreatedOverallQuality = nullptr;

		// Overall Quality
		{
			UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_OverallQuality"));
			OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_OverallQuality"));
			OverallQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			OverallQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			OverallQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			OverallQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			OverallQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediatly(true);

			GraphicsCategoryCollection->AddChildListData(OverallQuality);

			CreatedOverallQuality = OverallQuality;
		}

		// Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_ResolutionScale"));
			ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_ResolutionScale"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediatly(true);

			ResolutionScale->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ResolutionScale);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		// Global Illumination Quality
		{
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_GlobalIlluminationQuality"));
			GlobalIlluminationQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			GlobalIlluminationQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			GlobalIlluminationQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			GlobalIlluminationQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			GlobalIlluminationQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediatly(true);

			GlobalIlluminationQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		// Shadow Quality
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_ShadowQuality"));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_ShadowQuality"));
			ShadowQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			ShadowQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			ShadowQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			ShadowQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			ShadowQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediatly(true);

			ShadowQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		// AntiAliasing Quality
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_AntiAliasingQuality"));
			AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_AntiAliasing"));
			AntiAliasingQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			AntiAliasingQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			AntiAliasingQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			AntiAliasingQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			AntiAliasingQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplySettingsImmediatly(true);

			AntiAliasingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		// View Distance Quality
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_ViewDistanceQuality"));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_ViewDistance"));
			ViewDistanceQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Distance_Near"));
			ViewDistanceQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Distance_Medium"));
			ViewDistanceQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Distance_Far"));
			ViewDistanceQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Distance_VeryFar"));
			ViewDistanceQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Distance_Cinematic"));
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediatly(true);

			ViewDistanceQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		// Texture Quality
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_TextureQuality"));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_TextureQuality"));
			TextureQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			TextureQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			TextureQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			TextureQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			TextureQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediatly(true);

			TextureQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		// Visual Effect Quality
		{
			UListDataObject_StringInteger* VisualEffectQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
			VisualEffectQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_VisualEffectQuality"));
			VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_VisualEffectQuality"));
			VisualEffectQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			VisualEffectQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			VisualEffectQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			VisualEffectQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			VisualEffectQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplySettingsImmediatly(true);

			VisualEffectQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(VisualEffectQuality);

			GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
		}

		// Reflection Quality
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_ReflectionQuality"));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_ReflectionQuality"));
			ReflectionQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			ReflectionQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			ReflectionQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			ReflectionQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			ReflectionQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediatly(true);

			ReflectionQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		// Post Processing Quality
		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_PostProcessingQuality"));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_PostProcessingQuality"));
			PostProcessingQuality->AddIntegerOption(0, GET_DESCRIPTION("UI_Options_Value_Quality_Low"));
			PostProcessingQuality->AddIntegerOption(1, GET_DESCRIPTION("UI_Options_Value_Quality_Medium"));
			PostProcessingQuality->AddIntegerOption(2, GET_DESCRIPTION("UI_Options_Value_Quality_High"));
			PostProcessingQuality->AddIntegerOption(3, GET_DESCRIPTION("UI_Options_Value_Quality_Epic"));
			PostProcessingQuality->AddIntegerOption(4, GET_DESCRIPTION("UI_Options_Value_Quality_Cinematic"));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediatly(true);

			PostProcessingQuality->AddEditDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	// Advanced Graphics Category
	{
		UListDataObject_Collection* AdvancedGraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("AdvancedGraphicsCategoryCollection"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Category_AdvancedGraphics"));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		// Vertical Sync
		{
			UListDataObject_StringBool* VerticalSync = NewObject<UListDataObject_StringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_VerticalSync"));
			VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_VerticalSync"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplySettingsImmediatly(true);

			FOptionsDataEditConditionDescriptor FullscreenOnlyCondition;
			FullscreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()->bool
				{
					return (CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen);
				}
			);

			FullscreenOnlyCondition.SetDisabledRichReason(GET_DESCRIPTION("UI_Options_Warning_VSyncFullscreenOnly").ToString());
			FullscreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

			VerticalSync->AddEditCondition(FullscreenOnlyCondition);

			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		// Frame Rate Limit
		{
			UListDataObject_String* FrameRateLimit = NewObject<UListDataObject_String>();
			FrameRateLimit->SetDataID("FrameRateLimit");
			FrameRateLimit->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_FrameRateLimit"));
			FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_FrameRateLimit"));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), GET_DESCRIPTION("UI_Options_Value_FPS_30"));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), GET_DESCRIPTION("UI_Options_Value_FPS_60"));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), GET_DESCRIPTION("UI_Options_Value_FPS_90"));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), GET_DESCRIPTION("UI_Options_Value_FPS_120"));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), GET_DESCRIPTION("UI_Options_Value_FPS_Uncapped"));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplySettingsImmediatly(true);

			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}
	}

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab(ULocalPlayer* InOwiningLocalPlayer)
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Tab_Control"));

	UEnhancedInputLocalPlayerSubsystem* EISubsystem = InOwiningLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(EISubsystem);

	UEnhancedInputUserSettings* EIUserSettings = EISubsystem->GetUserSettings();
	check(EIUserSettings);

	// Keyboard Mouse Category
	{
		UListDataObject_Collection* KeyboardMouseCategoryCollection = NewObject<UListDataObject_Collection>();
		KeyboardMouseCategoryCollection->SetDataID(FName("KeyboardMouseCategoryCollection"));
		KeyboardMouseCategoryCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Category_KeyboardMouse"));

		ControlTabCollection->AddChildListData(KeyboardMouseCategoryCollection);

		// Keyboard Mouse Inputs
		{
			FPlayerMappableKeyQueryOptions KeyboardMouseOnly;
			KeyboardMouseOnly.KeyToMatch = EKeys::S; // Anykey
			KeyboardMouseOnly.bMatchBasicKeyTypes = true;

			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings) // Set
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, KeyboardMouseOnly))
						{
							/*Debug::Print(
								TEXT(" Mapping ID: ") + KeyMapping.GetMappingName().ToString() +
								TEXT(" Display Name: ") + KeyMapping.GetDisplayName().ToString() +
								TEXT(" Bound Key: ") + KeyMapping.GetCurrentKey().GetDisplayName().ToString()
							);*/

							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());

							FString DescKeyString = "UI_Options_Desc_" + KeyMapping.GetMappingName().ToString();
							FText DescText = FText::FromStringTable(TEXT("/Game/UI/StringTable/ST_OptionsScreenDescription"), DescKeyString);
							KeyRemapDataObject->SetDescriptionRichText(DescText);
							
							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::MouseAndKeyboard, KeyMapping);

							KeyboardMouseCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}
	}

	// Gamepad Category
	{
		UListDataObject_Collection* GamepadCategoryCollection = NewObject<UListDataObject_Collection>();
		GamepadCategoryCollection->SetDataID(FName("GamepadCategoryCollection"));
		GamepadCategoryCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Category_Gamepad"));

		ControlTabCollection->AddChildListData(GamepadCategoryCollection);

		// Gamepad Inputs
		{
			FPlayerMappableKeyQueryOptions GamepadOnly;
			GamepadOnly.KeyToMatch = EKeys::Gamepad_FaceButton_Bottom; // Anykey
			GamepadOnly.bMatchBasicKeyTypes = true;

			for (const TPair<FGameplayTag, UEnhancedPlayerMappableKeyProfile*>& ProfilePair : EIUserSettings->GetAllSavedKeyProfiles())
			{
				UEnhancedPlayerMappableKeyProfile* MappableKeyProfile = ProfilePair.Value;
				check(MappableKeyProfile);

				for (const TPair<FName, FKeyMappingRow>& MappingRowPair : MappableKeyProfile->GetPlayerMappingRows())
				{
					for (const FPlayerKeyMapping& KeyMapping : MappingRowPair.Value.Mappings) // Set
					{
						if (MappableKeyProfile->DoesMappingPassQueryOptions(KeyMapping, GamepadOnly))
						{
							//Debug::Print(
							//	TEXT(" Mapping ID: ") + KeyMapping.GetMappingName().ToString() +
							//	TEXT(" Display Name: ") + KeyMapping.GetDisplayName().ToString() +
							//	TEXT(" Bound Key: ") + KeyMapping.GetCurrentKey().GetDisplayName().ToString()
							//);

							UListDataObject_KeyRemap* KeyRemapDataObject = NewObject<UListDataObject_KeyRemap>();
							KeyRemapDataObject->SetDataID(KeyMapping.GetMappingName());
							KeyRemapDataObject->SetDataDisplayName(KeyMapping.GetDisplayName());

							FString DescKeyString = "UI_Options_Desc_" + KeyMapping.GetMappingName().ToString();

							//Debug::Print(DescKeyString);
							
							FText DescText = FText::FromStringTable(TEXT("/Game/UI/StringTable/ST_OptionsScreenDescription"), DescKeyString);
							KeyRemapDataObject->SetDescriptionRichText(DescText);

							KeyRemapDataObject->InitKeyRemapData(EIUserSettings, MappableKeyProfile, ECommonInputType::Gamepad, KeyMapping);

							GamepadCategoryCollection->AddChildListData(KeyRemapDataObject);
						}
					}
				}
			}
		}
	}

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

void UOptionsDataRegistry::InitSystemCollectionTab()
{
	UListDataObject_Collection* SystemTabCollection = NewObject<UListDataObject_Collection>();
	SystemTabCollection->SetDataID(FName("SystemTabCollection"));
	SystemTabCollection->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Tab_System"));

	// Language
	{
		UListDataObject_String* Language = NewObject<UListDataObject_String>();
		Language->SetDataID(FName("Language"));
		Language->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_Language"));
		Language->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_Language"));
		Language->AddDynamicOption(TEXT("ko"), GET_DESCRIPTION("UI_Options_Value_Ko"));
		Language->AddDynamicOption(TEXT("en"), GET_DESCRIPTION("UI_Options_Value_En"));
		Language->SetDefaultValueFromString(TEXT("ko"));
		Language->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentLanguage));
		Language->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentLanguage));
		Language->SetShouldApplySettingsImmediatly(true);

		SystemTabCollection->AddChildListData(Language);
	}

	//// Test Item
	//{
	//	UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
	//	TestItem->SetDataID(FName("TestItem"));
	//	TestItem->SetDataDisplayName(GET_DESCRIPTION("UI_Options_Name_TestItem"));
	//	TestItem->SetSoftDescriptionImage(UJAFunctionLibrary::GetOptionsSoftImageByTag(JAGameplayTags::Frontend_Image_TestImage));
	//	TestItem->SetDescriptionRichText(GET_DESCRIPTION("UI_Options_Desc_TestItem"));

	//	SystemTabCollection->AddChildListData(TestItem);
	//}

	RegisteredOptionsTabCollections.Add(SystemTabCollection);
}
