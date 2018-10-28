#include "InventoryList.h"

#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESFullName
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST

#include <exception>  // exception
#include <string>  // string

#include "Utility.h"  // numToHexString

#include "RE/TESObjectLIGH.h"  // RE::TESObjectLIGH


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
		if (isValidItem(a_entryData->type)) {
			_itemList.emplace_back(a_entryData);
		}
	}


	void InventoryList::add(InventoryEntryData* a_entryData, SInt32 a_count)
	{
		if (isValidItem(a_entryData->type)) {
			_itemList.emplace_back(a_entryData, a_count);
		}
	}


	void InventoryList::add(TESForm* a_form, SInt32 a_count)
	{
		InventoryEntryData* entryData = InventoryEntryData::Create(a_form, a_count);
		_heapList.push_back(entryData);
		add(entryData);
	}


	void InventoryList::clear()
	{
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


	InventoryList g_invList;
}
