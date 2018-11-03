#pragma once

#undef PlaySound

#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "Offsets.h"

#include "RE/PlayerControls.h"  // PlayerControls

class BGSSoundDescriptorForm;
class InputEvent;
class NiNode;
class NiPoint3;
class PlayerCharacter;
class PlayerInputHandler;

namespace RE
{
	class NiControllerManager;
	class NiControllerSequence;
	class NiNode;
	class TESObjectREFR;
}


namespace Hooks
{
	typedef void(*_PlayAnimation)(RE::TESObjectREFR* a_refr, RE::NiControllerManager* a_manager, RE::NiControllerSequence* a_toSeq, RE::NiControllerSequence* a_fromSeq, bool a_unk);
	extern RelocAddr<_PlayAnimation> PlayAnimation;

	typedef bool(*_PlaySound)(BGSSoundDescriptorForm* a_sound, UInt32 a_flag, const NiPoint3* a_position, NiNode* a_unk);
	extern RelocAddr<_PlaySound> PlaySound;

	typedef void(*_SendItemsPickPocketedEvent)(UInt32 numItems);	// Returns an int, but idk what it is
	extern RelocAddr<_SendItemsPickPocketedEvent> SendItemsPickPocketedEvent;

	void installHooks();
}
