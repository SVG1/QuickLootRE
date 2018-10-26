#pragma once

#include "skse64/GameExtraData.h"  // BSExtraData, EntryDataList
#include "skse64/GameReferences.h"  // TESObjectREFR

#include "Hooks.h"


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
			DEFINE_MEMBER_FN(ctor, ExtraContainerChanges::Data*, EXTRA_CONTAINER_CHANGES_DATA_CTOR, ::TESObjectREFR* ref);
			DEFINE_MEMBER_FN(InitContainer, void, EXTRA_CONTAINER_CHANGES_DATA_INIT_CONTAINER);
		};
		STATIC_ASSERT(offsetof(Data, objList)		== 0x00);
		STATIC_ASSERT(offsetof(Data, owner)			== 0x08);
		STATIC_ASSERT(offsetof(Data, totalWeight)	== 0x10);
		STATIC_ASSERT(offsetof(Data, armorWeight)	== 0x14);


		Data* data;
	};
	STATIC_ASSERT(offsetof(ExtraContainerChanges, data)	== 0x10);
	STATIC_ASSERT(sizeof(ExtraContainerChanges)			== 0x18);
}
