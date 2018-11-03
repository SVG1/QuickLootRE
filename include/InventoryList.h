#pragma once

#include <vector>  // vector

#include "ItemData.h"  // ItemData

class TESForm;

namespace RE
{
	class InventoryEntryData;
}


namespace QuickLootRE
{
	class InventoryList
	{
	public:
		InventoryList();
		~InventoryList();

		void									add(RE::InventoryEntryData* a_entryData);
		void									add(RE::InventoryEntryData* a_entryData, SInt32 a_count);
		void									add(TESForm* a_form, SInt32 a_count);

		void									sort();

		ItemData&								operator[](UInt32 a_pos)						{ return _itemList[a_pos]; }

		std::vector<ItemData>::iterator			begin() noexcept								{ return _itemList.begin(); }
		std::vector<ItemData>::iterator			end() noexcept									{ return _itemList.end(); }

		bool									empty()											{ return _itemList.empty(); }
		UInt32									size()											{ return _itemList.size(); }

		void clear();
		std::vector<ItemData>::iterator			erase(std::vector<ItemData>::iterator a_pos)	{ return _itemList.erase(a_pos); }

	private:
		bool									isValidItem(TESForm* a_item);

		void									quicksort(SInt32 a_lo, SInt32 a_hi);
		UInt64									partition(SInt32 a_lo, SInt32 a_hi);
		ItemData&								pivot(SInt32 a_lo, SInt32 a_hi);

		std::vector<ItemData>					_itemList;
		std::vector<RE::InventoryEntryData*>	_heapList;
	};


	extern InventoryList g_invList;
}
