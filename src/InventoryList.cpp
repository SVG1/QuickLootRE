#include "InventoryList.h"

#include "common/ITypes.h"  // UInt32
#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESFullName
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST

#include <vector>  // vector

#include "ItemData.h"  // ItemData


namespace QuickLootRE
{
	InventoryList::InventoryList()
	{}


	InventoryList::~InventoryList()
	{
		clear();
	}


	void InventoryList::add(InventoryEntryData* a_entryData)
	{
		static BSFixedString emptyStr = "";
		TESFullName* fullName = DYNAMIC_CAST(a_entryData->type, TESForm, TESFullName);
		if (fullName && fullName->name != emptyStr) {
			_itemList.emplace_back(a_entryData, fullName->name);
		}
	}


	void InventoryList::add(TESForm* a_form, UInt32 a_count)
	{
		InventoryEntryData* heapEntryData = InventoryEntryData::Create(a_form, a_count);
		_heapList.push_back(heapEntryData);
		add(heapEntryData);
	}


	void InventoryList::clear()
	{
		_itemList.clear();
		for (auto& entryData : _heapList) {
			entryData->Delete();
		}
		_heapList.clear();
	}


	InventoryList g_invList;
}
