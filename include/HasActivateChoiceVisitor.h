#pragma once

#include "RE/PerkEntryVisitor.h"  // RE::PerkEntryVisitor

namespace RE
{
	class Actor;
	class BGSPerkEntry;
	class TESObjectREFR;
}


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
