#pragma once

#include "skse64/GameExtraData.h"  // BSExtraData, ExtraContainerChanges, InventoryEntryData, InventoryEntryData::EquipData

#include "Offsets.h"

class BaseExtraList;
class TESForm;


namespace RE
{
	class EntryDataList;
	class InventoryEntryData;
	class TESObjectREFR;


	class ExtraContainerChanges : public BSExtraData
	{
	public:
		class Data
		{
		public:
			explicit Data(TESObjectREFR* ref)																			{ CALL_MEMBER_FN(this, ctor)(ref); }

			inline InventoryEntryData* FindItemEntry(TESForm* item)														{ return reinterpret_cast<InventoryEntryData*>(reinterpret_cast<::ExtraContainerChanges::Data*>(this)->FindItemEntry(item)); }
			inline InventoryEntryData* CreateEquipEntryData(TESForm* item)												{ return reinterpret_cast<InventoryEntryData*>(reinterpret_cast<::ExtraContainerChanges::Data*>(this)->CreateEquipEntryData(item)); }
			inline void GetEquipItemData(::InventoryEntryData::EquipData& stateOut, TESForm* item, SInt32 itemId)		{ reinterpret_cast<::ExtraContainerChanges::Data*>(this)->GetEquipItemData(stateOut, item, itemId); }
			inline void SetUniqueID(::BaseExtraList* itemList, TESForm* oldForm, TESForm* newForm)						{ CALL_MEMBER_FN(reinterpret_cast<::ExtraContainerChanges::Data*>(this), SetUniqueID)(itemList, oldForm, newForm); }
			inline void TransferItemUID(::BaseExtraList* extraList, TESForm* oldForm, TESForm* newForm, UInt32 unk1)	{ CALL_MEMBER_FN(reinterpret_cast<::ExtraContainerChanges::Data*>(this), TransferItemUID)(extraList, oldForm, newForm, unk1); }

			inline void InitContainer()																					{ CALL_MEMBER_FN(this, InitContainer)(); }


			EntryDataList*	objList;		// 00
			TESObjectREFR*	owner;			// 08
			float			totalWeight;	// 10
			float			armorWeight;	// 14
			bool			changed;		// 18
			UInt8			pad19[0x3];		// 19

		private:
			MEMBER_FN_PREFIX(Data);
			DEFINE_MEMBER_FN(ctor, ExtraContainerChanges::Data*, EXTRA_CONTAINER_CHANGES_DATA_CTOR, TESObjectREFR* ref);
			DEFINE_MEMBER_FN(InitContainer, void, EXTRA_CONTAINER_CHANGES_DATA_INIT_CONTAINER);
		};


		ExtraContainerChanges();
		virtual	~ExtraContainerChanges();

		inline ::ExtraContainerChanges::FoundEquipData	FindEquipped(FormMatcher& matcher, bool isWorn = true, bool isWornLeft = true)		{ return reinterpret_cast<::ExtraContainerChanges*>(this)->FindEquipped(matcher, isWorn, isWornLeft); }
		inline ::ExtraContainerChanges::FoundHotkeyData	FindHotkey(SInt32 hotkey)															{ return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(hotkey); }
		inline ::ExtraContainerChanges::FoundHotkeyData	FindHotkey(TESForm* form)															{ return reinterpret_cast<::ExtraContainerChanges*>(this)->FindHotkey(form); }


		Data*	data;	// 10
	};
}
