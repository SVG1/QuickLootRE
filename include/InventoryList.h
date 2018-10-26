#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESFullName
#include "skse64/GameForms.h"  // TESForm

#include <vector>  // vector

#include "ItemData.h"  // ItemData

namespace QuickLootRE
{
	class InventoryList
	{
	public:
		InventoryList();
		~InventoryList();

		void add(InventoryEntryData* a_entryData);
		void add(InventoryEntryData* a_entryData, SInt32 a_count);
		void add(TESForm* a_form, SInt32 a_count);

		ItemData& operator[](UInt32 a_pos) { return _itemList[a_pos]; }

		std::vector<ItemData>::iterator begin() noexcept { return _itemList.begin(); }
		std::vector<ItemData>::iterator end() noexcept { return _itemList.end(); }

		bool empty() { return _itemList.empty(); }
		UInt32 size() { return _itemList.size(); }

		void clear();
		std::vector<ItemData>::iterator erase(std::vector<ItemData>::iterator a_pos) { return _itemList.erase(a_pos); }

	private:
		TESFullName* getName(InventoryEntryData* a_entryData);

		std::vector<ItemData> _itemList;
		std::vector<InventoryEntryData*> _heapList;
	};


	extern InventoryList g_invList;
}
