#include "RE/ExtraContainerChanges.h"

#include "skse64/GameReferences.h"  // TESObjectREFR

#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR


namespace RE
{
	ExtraContainerChanges::Data::Data(TESObjectREFR* ref)
	{
		CALL_MEMBER_FN(this, ctor)(ref);
	}
}
