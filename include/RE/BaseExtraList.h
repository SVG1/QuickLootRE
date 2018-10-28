#pragma once

#include "skse64/GameBSExtraData.h"  // BaseExtraList

#include "Offsets.h"

#include "RE/ExtraContainerChanges.h"  // ExtraContainerChanges::Data

class BSExtraData;
class TESForm;


namespace RE
{
	class BaseExtraList
	{
	public:
		BaseExtraList();
		~BaseExtraList();

		inline bool							HasType(UInt32 type)										{ return reinterpret_cast<::BaseExtraList*>(this)->HasType(type); }
		inline void							MarkType(UInt32 type, bool bCleared)						{ reinterpret_cast<::BaseExtraList*>(this)->MarkType(type, bCleared); }
		inline bool							Remove(UInt8 type, BSExtraData* toRemove)					{ return reinterpret_cast<::BaseExtraList*>(this)->Remove(type, toRemove); }
		inline bool							Add(UInt8 type, BSExtraData* toAdd)							{ return reinterpret_cast<::BaseExtraList*>(this)->Add(type, toAdd); }
		inline bool							CheckContainerExtraData(bool isEquipped)					{ return reinterpret_cast<::BaseExtraList*>(this)->CheckContainerExtraData(isEquipped); }
		inline const char*					GetDisplayName(TESForm* type)								{ return reinterpret_cast<::BaseExtraList*>(this)->GetDisplayName(type); }
		inline BSExtraData*					GetByType(UInt32 type)										{ return reinterpret_cast<::BaseExtraList*>(this)->GetByType(type); }

		inline void							SetInventoryChanges(ExtraContainerChanges::Data* changes)	{ CALL_MEMBER_FN(this, SetInventoryChanges_Impl)(changes); }
		inline UInt32						GetAshPileRefHandle(UInt32& refHandle)						{ return CALL_MEMBER_FN(this, GetAshPileRefHandle_Impl)(refHandle); }


		BSExtraData*						m_data;		// 00
		::BaseExtraList::PresenceBitfield*	m_presence;	// 08
		BSReadWriteLock						m_lock;

	private:
		MEMBER_FN_PREFIX(BaseExtraList);
		DEFINE_MEMBER_FN(SetInventoryChanges_Impl, void, BASE_EXTRA_LIST_SET_INVENTORY_CHANGES_IMPL, ExtraContainerChanges::Data* changes);
		DEFINE_MEMBER_FN(GetAshPileRefHandle_Impl, UInt32, BASE_EXTRA_LIST_GET_ASH_PILE_REF_HANDLE_IMPL, UInt32& refHandle);
	};
	STATIC_ASSERT(sizeof(BaseExtraList) == 0x18);
}
