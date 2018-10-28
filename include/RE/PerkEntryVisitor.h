#pragma once

#include "BGSPerkEntry.h"  // RE::BGSPerkEntry


namespace RE
{
	class PerkEntryVisitor
	{
	public:
		virtual UInt32 Visit(BGSPerkEntry* perkEntry) = 0;
	};
}
