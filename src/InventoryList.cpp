#include "InventoryList.h"

#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESFullName
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST

#include <exception>  // exception
#include <string>  // string

#include "Utility.h"  // numToHexString

#include "RE/InventoryEntryData.h"  // RE::InventoryEntryData
#include "RE/TESObjectLIGH.h"  // RE::TESObjectLIGH


namespace QuickLootRE
{
	InventoryList::InventoryList()
	{}


	InventoryList::~InventoryList()
	{
		clear();
	}


	void InventoryList::add(RE::InventoryEntryData* a_entryData)
	{
		if (isValidItem(a_entryData->type)) {
			_itemList.emplace_back(a_entryData);
		}
	}


	void InventoryList::add(RE::InventoryEntryData* a_entryData, SInt32 a_count)
	{
		if (isValidItem(a_entryData->type)) {
			_itemList.emplace_back(a_entryData, a_count);
		}
	}


	void InventoryList::add(TESForm* a_form, SInt32 a_count)
	{
		RE::InventoryEntryData* entryData = RE::InventoryEntryData::Create(a_form, a_count);
		_heapList.push_back(entryData);
		add(entryData);
	}


	void InventoryList::sort()
	{
		quicksort(0, _itemList.size() - 1);
	}


	void InventoryList::clear()
	{
		ItemData::setContainer(0);
		_itemList.clear();
		for (auto& entryData : _heapList) {
			entryData->Delete();
		}
		_heapList.clear();
	}


	bool InventoryList::isValidItem(TESForm* a_item)
	{
		if (!a_item) {
			return false;
		}

		if (a_item->formType == kFormType_LeveledItem) {
			return false;
		}

		if (a_item->formType == kFormType_Light) {
			RE::TESObjectLIGH* light = static_cast<RE::TESObjectLIGH*>(a_item);
			if (!light->CanBeCarried()) {
				return false;
			}
		} else if (!a_item->IsPlayable()) {
			return false;
		}

		TESFullName* fullName = 0;
		try {
			static BSFixedString emptyStr = "";
			fullName = DYNAMIC_CAST(a_item, TESForm, TESFullName);
			if (!fullName || fullName->name == emptyStr) {
				return false;
			}
		} catch (std::exception& e) {
			std::string msg = "[ERROR] Form (0x" + numToHexString(a_item->formID) + ") does not have TESFullName (" + std::to_string(a_item->formType) + ")";
			_ERROR(msg.c_str());
			_ERROR(e.what());
			return false;
		}

		return true;
	}


	void InventoryList::quicksort(SInt32 a_lo, SInt32 a_hi)
	{
		if (a_lo < a_hi) {
			SInt32 p = partition(a_lo, a_hi);
			quicksort(a_lo, p - 1);
			quicksort(p + 1, a_hi);
		}
	}


	UInt64 InventoryList::partition(SInt32 a_lo, SInt32 a_hi)
	{
		ItemData& p = pivot(a_lo, a_hi);
		SInt32 i = a_lo;
		for (SInt32 j = a_lo; j < a_hi - 1; ++j) {
			if (_itemList[j] < p) {
				if (i != j) {
					swap(_itemList[i], _itemList[j]);
				}
				++i;
			}
		}
		swap(_itemList[i], _itemList[a_hi]);
		return i;
	}


	ItemData& InventoryList::pivot(SInt32 a_lo, SInt32 a_hi)
	{
		SInt32 mid = a_lo + (a_hi - a_lo) / 2;
		if (_itemList[mid] < _itemList[a_lo]) {
			swap(_itemList[mid], _itemList[a_lo]);
		}
		if (_itemList[a_hi] < _itemList[a_lo]) {
			swap(_itemList[a_hi], _itemList[a_lo]);
		}
		if (_itemList[mid] < _itemList[a_hi]) {
			swap(_itemList[mid], _itemList[a_hi]);
		}
		return _itemList[a_hi];
	}


	InventoryList g_invList;
}
