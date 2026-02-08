// JhLim All Rights Reserved


#include "Widgets/Frontend/Options/OptionsDataRegistry.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Frontend/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Frontend/Options/OptionsDataInteractionHelper.h"
#include "JASettings/JAGameUserSettings.h"
#include "JAFunctionLibrary.h"
#include "JAGameplayTags.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UJAGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwiningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
	InitSystemCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsBySelectedTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *InSelectedTabID.ToString());

	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	return FoundTabCollection->GetAllChildListData();
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("게임플레이")));

	// This is the full code for contructor data interactor helper
	/*TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper =
		MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UJAGameUserSettings, GetCurrentGameDifficulty));*/

	// Game Difficulty
	{
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("난이도")));

		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("게임 난이도를 조절합니다.\n\n<Bold>쉬움:</> 전투 부담이 적고 가장 여유로운 플레이가 가능합니다.\n\n<Bold>보통:</> 표준적인 전투 난이도로 적절한 긴장감을 제공합니다.\n\n<Bold>어려움:</> 더 강력한 적들이 등장하며 정교한 조작을 요구합니다.\n\n<Bold>매우 어려움:</> 극한의 도전적인 전투를 제공하며, 1회차 플레이 시 권장하지 않습니다.")));

		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("쉬움")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("보통")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("어려움")));
		GameDifficulty->AddDynamicOption(TEXT("Very Hard"), FText::FromString(TEXT("매우 어려움")));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));

		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));

		GameDifficulty->SetShouldApplySettingsImmediatly(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	// Test Item
	{
		UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString(TEXT("Text Image Item")));
		TestItem->SetSoftDescriptionImage(UJAFunctionLibrary::GetOptionsSoftImageByTag(JAGameplayTags::Frontend_Image_TestImage));
		TestItem->SetDescriptionRichText(FText::FromString(TEXT("테스트 이미지 입니다.")));

		GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("사운드")));

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("그래픽")));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlTabCollection = NewObject<UListDataObject_Collection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("조작")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}

void UOptionsDataRegistry::InitSystemCollectionTab()
{
	UListDataObject_Collection* SystemTabCollection = NewObject<UListDataObject_Collection>();
	SystemTabCollection->SetDataID(FName("SystemTabCollection"));
	SystemTabCollection->SetDataDisplayName(FText::FromString(TEXT("시스템")));

	RegisteredOptionsTabCollections.Add(SystemTabCollection);
}
