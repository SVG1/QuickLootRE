#pragma once

#include "common/ITypes.h"  // UInt32

#include "RE/Actor.h"
#include "RE/BGSEntryPointFunctionDataActivateChoice.h"  // RE::BGSEntryPointFunctionDataActivateChoice
#include "RE/BGSPerkEntry.h"  // RE::BGSPerkEntry
#include "RE/PerkEntryVisitor.h"  // RE::PerkEntryVisitor
#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR


namespace QuickLootRE
{
	class HasActivateChoiceVisitor : public RE::PerkEntryVisitor
	{
	private:
		enum PerkFormID : UInt32
		{
			kPerkFormID_VampireFeed = 0x000CF02C,
			kPerkFormID_Cannibalism = 0x000EE5C3
		};


	public:
		HasActivateChoiceVisitor(RE::Actor* a_actor, RE::TESObjectREFR* a_target);

		virtual UInt32	Visit(RE::BGSPerkEntry* perkEntry) override;
		inline bool		GetResult() const { return _result; }

	protected:
		RE::Actor*			_actor;
		RE::TESObjectREFR*	_target;
		bool				_result;
	};
}
