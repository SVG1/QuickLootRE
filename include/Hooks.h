#pragma once

#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "Offsets.h"

#include "RE/PlayerControls.h"  // PlayerControls

class PlayerCharacter;


namespace Hooks
{
	typedef void _StartActivation_Fn(PlayerCharacter* a_thisPlayerCharacter);
	extern RelocAddr<_StartActivation_Fn*> StartActivation_Fn;
	extern RelocPtr<uintptr_t> StartActivation_Call;

	typedef void(*_ReadyWeaponHandler_ProcessButton)(RE::PlayerControls* a_handler, ButtonEvent* a_event, RE::PlayerControls::Data024* a_data);
	extern RelocPtr<_ReadyWeaponHandler_ProcessButton> vtbl_ReadyWeaponHandler_ProcessButton;
	extern _ReadyWeaponHandler_ProcessButton orig_ReadyWeaponHandler_ProcessButton;


	void installHooks();
}
