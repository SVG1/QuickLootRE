#pragma once

#include "skse64/GameBSExtraData.h"  // BaseExtraList, BSExtraData
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameForms.h"  // TESForm

#include "Hooks.h"

#include "RE_ExtraContainerChanges.h"  // RE::ExtraContainerChanges::Data


namespace RE
{
	class BaseExtraList
	{
	public:
		BaseExtraList();
		~BaseExtraList();

		bool								HasType(UInt32 type);
		void								MarkType(UInt32 type, bool bCleared);
		bool								Remove(UInt8 type, BSExtraData* toRemove);
		bool								Add(UInt8 type, BSExtraData* toAdd);
		bool								CheckContainerExtraData(bool isEquipped);
		const char*							GetDisplayName(TESForm* type);
		BSExtraData*						GetByType(UInt32 type);

		void								SetInventoryChanges(ExtraContainerChanges::Data* changes);


		BSExtraData*						m_data;		// 00
		::BaseExtraList::PresenceBitfield*	m_presence;	// 08
		BSReadWriteLock						m_lock;

	private:
		MEMBER_FN_PREFIX(BaseExtraList);
		DEFINE_MEMBER_FN(SetInventoryChanges_Impl, void, BASE_EXTRA_LIST_SET_INVENTORY_CHANGES_IMPL, ExtraContainerChanges::Data* changes);
	};
	STATIC_ASSERT(sizeof(BaseExtraList) == 0x18);


	inline bool BaseExtraList::HasType(UInt32 type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->HasType(type);
	}


	inline void BaseExtraList::MarkType(UInt32 type, bool bCleared)
	{
		reinterpret_cast<::BaseExtraList*>(this)->MarkType(type, bCleared);
	}


	inline bool BaseExtraList::Remove(UInt8 type, BSExtraData* toRemove)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->Remove(type, toRemove);
	}


	inline bool BaseExtraList::Add(UInt8 type, BSExtraData* toAdd)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->Add(type, toAdd);
	}


	inline bool BaseExtraList::CheckContainerExtraData(bool isEquipped)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->CheckContainerExtraData(isEquipped);
	}


	inline const char* BaseExtraList::GetDisplayName(TESForm* type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->GetDisplayName(type);
	}


	inline BSExtraData* BaseExtraList::GetByType(UInt32 type)
	{
		return reinterpret_cast<::BaseExtraList*>(this)->GetByType(type);
	}


	inline void BaseExtraList::SetInventoryChanges(ExtraContainerChanges::Data* changes)
	{
		CALL_MEMBER_FN(this, SetInventoryChanges_Impl)(changes);
	}
}
