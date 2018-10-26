#include "Hooks.h"

#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameReferences.h"  // PlayerCharacter
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "LootMenu.h"


namespace QuickLootRE
{
	RelocPtr<uintptr_t> startActivationAddr(0x00708FCF);				// 1_5_50
	typedef void _StartActivationFunc(PlayerCharacter* a_player);
	RelocAddr<_StartActivationFunc*> startActivationFunc(0x006AA180);	// 1_5_50


	// ExtraContainerChanges::Data::
	typedef ExtraContainerChanges::Data* ECCData_ctor_t(ExtraContainerChanges::Data* o, TESObjectREFR* ref);
	typedef void ECCData_dtor_t(ExtraContainerChanges::Data* o);
	typedef void ECCData_InitContainer_t(ExtraContainerChanges::Data* o);

	extern RelocAddr<ECCData_ctor_t*>                        ECCData_ctor;
	extern RelocAddr<ECCData_dtor_t*>                        ECCData_dtor;
	extern RelocAddr<ECCData_InitContainer_t*>               ECCData_InitContainer;

	// BaseExtraList::
	typedef void BaseExtraList_SetInventoryChanges_t(BaseExtraList* o, ExtraContainerChanges::Data* changes);
	typedef SInt16 BaseExtraList_GetItemCount_t(const BaseExtraList* o);

	extern RelocAddr<BaseExtraList_SetInventoryChanges_t*>   BaseExtraList_SetInventoryChanges;
	extern RelocAddr<BaseExtraList_GetItemCount_t*>			 BaseExtraList_GetItemCount;


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
