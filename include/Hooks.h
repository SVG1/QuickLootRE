#pragma once

#include "skse64/GameReferences.h"  // PlayerCharacter
#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "RE_MenuEventHandler.h"  // RE::MenuEventHandler


#define EXTRA_CONTAINER_CHANGES_DATA_CTOR			0x001D93F0	// 1_5_50
#define EXTRA_CONTAINER_CHANGES_DATA_INIT_CONTAINER	0x001E9F80	// 1_5_50

#define	BASE_EXTRA_LIST_SET_INVENTORY_CHANGES_IMPL	0x0010F7B0	// 1_5_50

#define START_ACTIVATION_ADDR						0x00708FCF	// 1_5_50
#define START_ACTIVATION_FUNC						0x006AA180	// 1_5_50

#define REGISTER_MENU_EVENT_HANDLER					0x008A8110	// 1_5_50
#define REMOVE_MENU_EVENT_HANDLER					0x008A81E0	// 1_5_50


namespace QuickLootRE
{
	extern RelocPtr<uintptr_t> startActivationAddr;

	typedef void _StartActivationFunc(PlayerCharacter* a_player);
	extern RelocAddr<_StartActivationFunc*> startActivationFunc;

	typedef void RegisterMenuEventHandler_t(MenuControls*, RE::MenuEventHandler*);
	extern RelocAddr<RegisterMenuEventHandler_t*> RegisterMenuEventHandler;

	typedef void RemoveMenuEventHandler_t(MenuControls *, RE::MenuEventHandler *);
	extern RelocAddr<RemoveMenuEventHandler_t*> RemoveMenuEventHandler;


	void startActivation(PlayerCharacter* a_player);
	void installHooks();
}
