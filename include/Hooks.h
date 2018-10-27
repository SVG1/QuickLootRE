#pragma once

#include "skse64/GameReferences.h"  // PlayerCharacter
#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "Offsets.h"

#include "RE/MenuEventHandler.h"  // RE::MenuEventHandler
#include "RE/PlayerControls.h"  // RE::PlayerControls


namespace Hooks
{
	extern RelocPtr<uintptr_t> StartActivationOldCall;

	typedef void FnStartActivation_t(PlayerCharacter* a_thisPlayerCharacter);
	extern RelocAddr<FnStartActivation_t*> fnStartActivationOld;

	typedef void RegisterMenuEventHandler_t(MenuControls* a_thisMenuControls, RE::MenuEventHandler* a_menuHandler);
	extern RelocAddr<RegisterMenuEventHandler_t*> RegisterMenuEventHandler;

	typedef void RemoveMenuEventHandler_t(MenuControls* a_thisMenuControls, RE::MenuEventHandler* a_menuHandler);
	extern RelocAddr<RemoveMenuEventHandler_t*> RemoveMenuEventHandler;

	RelocPtr<uintptr_t> ReadyWeaponHandlerVFTable_ProcessButton_Hook(MOVEMENT_HANDLER_VFTABLE_META + 0x17 * 8);
	typedef void ReadyWeaponHandler_ProcessButton_t(RE::PlayerControls* a_handler, ButtonEvent* a_event, RE::PlayerControls::Data024* a_data);
	RelocAddr<ReadyWeaponHandler_ProcessButton_t*> ReadyWeaponHandler_ProcessButton(0x00709500);


	void StartActivation_Hook(PlayerCharacter* a_player);
	void InstallHooks();
}
