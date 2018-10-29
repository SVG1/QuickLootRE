#pragma once

#undef PlaySound

#include "skse64_common/Relocation.h"  // RelocPtr, RelocAddr

#include "Offsets.h"

#include "RE/PlayerControls.h"  // PlayerControls

class BGSSoundDescriptorForm;
class NiNode;
class NiPoint3;
class PlayerCharacter;

namespace RE
{
	class NiControllerManager;
	class NiControllerSequence;
	class NiNode;
	class TESObjectREFR;
}


namespace Hooks
{
	typedef void _StartActivation_Fn(PlayerCharacter* a_thisPlayerCharacter);
	extern RelocAddr<_StartActivation_Fn*> StartActivation_Fn;
	extern RelocPtr<uintptr_t> StartActivation_Call;

	typedef void(*_ReadyWeaponHandler_ProcessButton)(RE::PlayerControls* a_handler, ButtonEvent* a_event, RE::PlayerControls::Data024* a_data);
	extern RelocPtr<_ReadyWeaponHandler_ProcessButton> vtbl_ReadyWeaponHandler_ProcessButton;
	extern _ReadyWeaponHandler_ProcessButton orig_ReadyWeaponHandler_ProcessButton;

	typedef void(*_PlayAnimation)(RE::TESObjectREFR* refr, RE::NiControllerManager* manager, RE::NiControllerSequence* toSeq, RE::NiControllerSequence* fromSeq, bool unk);
	extern RelocAddr<_PlayAnimation> PlayAnimation;

	typedef bool(*_PlaySound)(BGSSoundDescriptorForm* sound, UInt32 flag, const NiPoint3* position, NiNode* unk);
	extern RelocAddr<_PlaySound> PlaySound;

	void installHooks();
}
