#include "Hooks.h"

#include "skse64/GameReferences.h"  // PlayerCharacter
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline

#include "LootMenu.h"  // LootMenu


namespace QuickLootRE
{
	RelocPtr<uintptr_t> startActivationAddr(START_ACTIVATION_ADDR);
	RelocAddr<_StartActivationFunc*> startActivationFunc(START_ACTIVATION_FUNC);
	RelocAddr<RegisterMenuEventHandler_t*> RegisterMenuEventHandler(REGISTER_MENU_EVENT_HANDLER);
	RelocAddr<RemoveMenuEventHandler_t*> RemoveMenuEventHandler(REMOVE_MENU_EVENT_HANDLER);


	void startActivation(PlayerCharacter* a_player)
	{
		if (LootMenu::GetSingleton()) {
			LootMenu::TakeItem();
		} else {
			startActivationFunc(a_player);
		}
	}


	void installHooks()
	{
		g_branchTrampoline.Write5Call(startActivationAddr.GetUIntPtr(), GetFnAddr(startActivation));
	}
}
