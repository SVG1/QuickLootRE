#pragma once

#include "common/ITypes.h"  // UInt8, UInt32
#include "skse64/GameBSExtraData.h"  // BaseExtraList, BSExtraData
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameForms.h"  // TESForm

#include "Offsets.h"

#include "RE_ExtraContainerChanges.h"  // RE::ExtraContainerChanges::Data


namespace RE
{
	class BaseExtraList
	{
	public:
		BaseExtraList();
		~BaseExtraList();

		bool								HasType(UInt32 type)										{ return reinterpret_cast<::BaseExtraList*>(this)->HasType(type); }
		void								MarkType(UInt32 type, bool bCleared)						{ reinterpret_cast<::BaseExtraList*>(this)->MarkType(type, bCleared); }
		bool								Remove(UInt8 type, BSExtraData* toRemove)					{ return reinterpret_cast<::BaseExtraList*>(this)->Remove(type, toRemove); }
		bool								Add(UInt8 type, BSExtraData* toAdd)							{ return reinterpret_cast<::BaseExtraList*>(this)->Add(type, toAdd); }
		bool								CheckContainerExtraData(bool isEquipped)					{ return reinterpret_cast<::BaseExtraList*>(this)->CheckContainerExtraData(isEquipped); }
		const char*							GetDisplayName(TESForm* type)								{ return reinterpret_cast<::BaseExtraList*>(this)->GetDisplayName(type); }
		BSExtraData*						GetByType(UInt32 type)										{ return reinterpret_cast<::BaseExtraList*>(this)->GetByType(type); }

		void								SetInventoryChanges(ExtraContainerChanges::Data* changes)	{ CALL_MEMBER_FN(this, SetInventoryChanges_Impl)(changes); }


		BSExtraData*						m_data;		// 00
		::BaseExtraList::PresenceBitfield*	m_presence;	// 08
		BSReadWriteLock						m_lock;

	private:
		MEMBER_FN_PREFIX(BaseExtraList);
		DEFINE_MEMBER_FN(SetInventoryChanges_Impl, void, BASE_EXTRA_LIST_SET_INVENTORY_CHANGES_IMPL, ExtraContainerChanges::Data* changes);
	};
	STATIC_ASSERT(sizeof(BaseExtraList) == 0x18);
}
