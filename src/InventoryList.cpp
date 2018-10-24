#include "InventoryList.h"

#include "common/ITypes.h"  // UInt32
#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameForms.h"  // TESForm

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
		_itemList.emplace_back(a_entryData);
	}


	void InventoryList::add(TESForm* a_form, UInt32 a_count)
	{
		InventoryEntryData* heapEntryData = InventoryEntryData::Create(a_form, a_count);
		_itemList.emplace_back(heapEntryData);
		_heapList.push_back(heapEntryData);
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