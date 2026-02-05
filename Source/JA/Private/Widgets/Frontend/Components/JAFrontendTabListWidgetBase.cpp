// JhLim All Rights Reserved


#include "Widgets/Frontend/Components/JAFrontendTabListWidgetBase.h"
#include "Editor/WidgetCompilerLog.h"
#include "Widgets/Frontend/Components/JAFrontendCommonButtonBase.h"

#if WITH_EDITOR	
void UJAFrontendTabListWidgetBase::ValidateCompiledDefaults(IWidgetCompilerLog& CompileLog) const
{
	Super::ValidateCompiledDefaults(CompileLog);

	if (!TabButtonEntryWidgetClass)
	{
		CompileLog.Error(FText::FromString(
			TEXT("The variable TabButtonEntryWidgetClass has no valid entry specified. ") +
			GetClass()->GetName() +
			TEXT(" needs a valid entry widget class to function properly")
		));
	}
}
#endif
