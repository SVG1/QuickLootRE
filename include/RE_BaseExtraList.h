#pragma once

#include "skse64/GameExtraData.h"  // ExtraContainerChanges

#include "RE_ExtraContainerChanges.h"  // RE::ExtraContainerChanges::Data


namespace RE
{
	class BaseExtraList
	{
	public:
		void SetInventoryChanges(RE::ExtraContainerChanges::Data* changes)
		{
			CALL_MEMBER_FN(this, SetInventoryChanges_Impl)(changes);
		}

	private:
		MEMBER_FN_PREFIX(BaseExtraList);
		DEFINE_MEMBER_FN(SetInventoryChanges_Impl, void, 0x0010F7B0, RE::ExtraContainerChanges::Data* changes);	// 1_5_50
	};
}
