#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/GameForms.h"  // BGSPerk
#include "skse64/GameObjects.h"  // SpellItem
#include "skse64/GameTypes.h"  // BSFixedString

#include "RE/BGSEntryPointFunctionData.h"  // RE::BGSEntryPointFunctionData


namespace RE
{
	class BGSEntryPointFunctionDataActivateChoice : public BGSEntryPointFunctionData
	{
	public:
		enum Flags : UInt32
		{
			kFlag_RunImmediately = 1,
			kFlag_ReplaceDefault = 2
		};


		virtual ~BGSEntryPointFunctionDataActivateChoice();

		BSFixedString	label;
		BGSPerk*		perk;
		SpellItem*		appliedSpell;
		UInt32			flags;
		UInt32			unk14;
	};
}
