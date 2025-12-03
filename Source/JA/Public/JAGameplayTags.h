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
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack_Axe);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_HeavyAttack_Axe);

	/* Player Tags */
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Equip_Axe);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Unquip_Axe);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Light_Axe);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_Attack_Heavy_Axe);

	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Weapon_Axe);

	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_EquipAxe);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Event_Unequip_Axe);
		
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Status_JumpToFinisher);

	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Light);
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_SetByCaller_AttackType_Heavy);


	/* Enemy Tags */
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Enemy_Weapon);

	/* Shared Tags */
	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);

	JA_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_SetByCaller_BaseDamage);
}