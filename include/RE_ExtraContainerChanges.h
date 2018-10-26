#pragma once

#include "skse64/GameExtraData.h"


namespace RE
{
	class ExtraContainerChanges : public ::BSExtraData
	{
	public:
		ExtraContainerChanges();
		virtual ~ExtraContainerChanges();

		class Data
		{
		public:
			explicit Data(::TESObjectREFR* ref)
			{
				CALL_MEMBER_FN(this, ctor)(ref);
			}

			void InitContainer()
			{
				CALL_MEMBER_FN(this, InitContainer)();
			}

			::EntryDataList*	objList;
			::TESObjectREFR*	owner;
			float				totalWeight;
			float				armorWeight;

		private:
			MEMBER_FN_PREFIX(Data);
			DEFINE_MEMBER_FN(ctor, ExtraContainerChanges::Data*, 0x001D93F0, ::TESObjectREFR* ref);	// 1_5_50
			DEFINE_MEMBER_FN(InitContainer, void, 0x001E9F80);	// 1_5_50
		};

		Data* data;
	};
}
