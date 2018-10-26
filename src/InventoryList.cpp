#include "InventoryList.h"

#include "common/ITypes.h"  // UInt32
#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESFullName
#include "skse64/GameForms.h"  // TESForm
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST

#include <exception>  // exception
#include <vector>  // vector
#include <string>  // string

#include "ItemData.h"  // ItemData
#include "Utility.h"  // numToHexString


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
		TESFullName* fullName = 0;
		if (fullName = getName(a_entryData)) {
			_itemList.emplace_back(a_entryData, fullName->name);
		}
	}


	void InventoryList::add(InventoryEntryData* a_entryData, SInt32 a_count)
	{
		TESFullName* fullName = 0;
		if (fullName = getName(a_entryData)) {
			_itemList.emplace_back(a_entryData, fullName->name, a_count);
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


	TESFullName* InventoryList::getName(InventoryEntryData* a_entryData)
	{
		static BSFixedString emptyStr = "";
		TESFullName* fullName = 0;

		try {
			fullName = DYNAMIC_CAST(a_entryData->type, TESForm, TESFullName);
			if (fullName && fullName->name != emptyStr) {
				return fullName;
			} else {
				return 0;
			}
		} catch (std::exception& e) {
			std::string msg = "[DEBUG] Form (0x" + numToHexString(a_entryData->type->formID) + ") does not have TESFullName (" + std::to_string(a_entryData->type->formType) + ")";
			_DMESSAGE(msg.c_str());
			return 0;
		}
	}


	InventoryList g_invList;
}
