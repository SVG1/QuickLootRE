#include "Hooks.h"

#include "skse64/GameReferences.h"  // PlayerCharacter
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "LootMenu.h"


namespace QuickLootRE
{
	RelocPtr<uintptr_t> startActivationAddr(0x00708FCF);				// 1_5_50
	typedef void _StartActivationFunc(PlayerCharacter* a_player);
	RelocAddr<_StartActivationFunc*> startActivationFunc(0x006AA180);	// 1_5_50


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
