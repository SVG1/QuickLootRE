#include "Hooks.h"

#include "skse64/GameReferences.h"  // PlayerCharacter
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline

#include "LootMenu.h"  // LootMenu
#include "Offsets.h"


namespace Hooks
{
	RelocPtr<uintptr_t> StartActivationOldCall(START_ACTIVATION_ADDR);
	RelocAddr<FnStartActivation_t*> fnStartActivationOld(START_ACTIVATION_FUNC);
	RelocAddr<RegisterMenuEventHandler_t*> RegisterMenuEventHandler(REGISTER_MENU_EVENT_HANDLER);
	RelocAddr<RemoveMenuEventHandler_t*> RemoveMenuEventHandler(REMOVE_MENU_EVENT_HANDLER);


	void StartActivation_Hook(PlayerCharacter* a_player)
	{
		if (QuickLootRE::LootMenu::GetSingleton()) {
			QuickLootRE::LootMenu::TakeItem();
		} else {
			fnStartActivationOld(a_player);
		}
	}


	void InstallHooks()
	{
		g_branchTrampoline.Write5Call(StartActivationOldCall.GetUIntPtr(), GetFnAddr(StartActivation_Hook));
	}
}
