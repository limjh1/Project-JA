// JhLim All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

namespace JAGameplayTags
{
	/* Input Tags */
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_EquipAxe);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_UnequipAxe);

	/* Player Tags */
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Equip_Axe);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);
}