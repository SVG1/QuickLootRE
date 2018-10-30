#include "Hooks.h"

#include "skse64/GameReferences.h"  // g_thePlayer
#include "skse64_common/BranchTrampoline.h"  // g_branchTrampoline
#include "skse64_common/SafeWrite.h"  // SafeWrite64

#include "LootMenu.h"  // LootMenu
#include "Offsets.h"

#include "RE/BSWin32GamepadDevice.h"  // RE::BSWin32GamepadDevice
#include "RE/PlayerCharacter.h"  // RE::PlayerCharacter
#include "RE/PlayerControls.h"  // PlayerControls, PlayerControls::Data024
#include "RE/PlayerInputHandler.h"  // RE::PlayerInputHandler
#include "RE/ReadyWeaponHandler.h"  // RE::ReadyWeaponHandler


class PlayerCharacter;


namespace Hooks
{
	RelocPtr<uintptr_t> StartActivation_Call(PLAYER_CHARACTER_START_ACTIVATION_FN);

	RelocAddr<_PlayAnimation> PlayAnimation(PLAY_ANIMATION);
	RelocAddr<_PlaySound> PlaySound(PLAY_SOUND);


	template <uintptr_t offset>
	class CameraStateHandler : public RE::PlayerInputHandler
	{
	public:
		typedef bool(CameraStateHandler::*_CanProcess)(InputEvent* a_event);
		static _CanProcess orig_CanProcess;


		bool hook_CanProcess(InputEvent* a_event)
		{
			static InputStringHolder* holder = InputStringHolder::GetSingleton();

			bool result = (this->*orig_CanProcess)(a_event);
			if (result && a_event && QuickLootRE::LootMenu::GetSingleton()) {
				result = (a_event->GetControlID() == &holder->togglePOV);
			}

			return result;
		}


		static void installHook()
		{
			RelocPtr<_CanProcess> vtbl_CanProcess(offset);
			orig_CanProcess = *vtbl_CanProcess;
			SafeWrite64(vtbl_CanProcess.GetUIntPtr(), GetFnAddr(&hook_CanProcess));
		}
	};


	template <uintptr_t offset> typename CameraStateHandler<offset>::_CanProcess CameraStateHandler<offset>::orig_CanProcess;
	typedef CameraStateHandler<FIRST_PERSON_STATE_VTBL_META + 0x58> FirstPersonStateHandler;
	typedef CameraStateHandler<THIRD_PERSON_STATE_VTBL_META + 0x90> ThirdPersonStateHandler;


	class FavoritesHandler : public RE::MenuEventHandler
	{
	public:
		typedef bool(FavoritesHandler::*_CanProcess)(InputEvent* a_event);
		static _CanProcess orig_CanProcess;


		bool hook_CanProcess(InputEvent* a_event)
		{
			typedef RE::BSWin32GamepadDevice::Gamepad Gamepad;

			bool result = (this->*orig_CanProcess)(a_event);
			if (result && a_event && QuickLootRE::LootMenu::GetSingleton()) {
				if (a_event->deviceType == kDeviceType_Gamepad && a_event->eventType == InputEvent::kEventType_Button) {
					ButtonEvent* button = static_cast<ButtonEvent*>(a_event);
					result = (button->keyMask != Gamepad::kGamepad_Up && button->keyMask != Gamepad::kGamepad_Down);
				}
			}

			return result;
		}


		static void installHook()
		{
			RelocPtr<_CanProcess> vtbl_CanProcess(FAVORITES_HANDLER_VTBL_META + 0x8);
			orig_CanProcess = *vtbl_CanProcess;
			SafeWrite64(vtbl_CanProcess.GetUIntPtr(), GetFnAddr(&hook_CanProcess));
		}
	};


	FavoritesHandler::_CanProcess FavoritesHandler::orig_CanProcess;


	class ReadyWeaponHandlerEx : public RE::ReadyWeaponHandler
	{
	public:
		typedef void(ReadyWeaponHandlerEx::*_ProcessButton)(ButtonEvent* a_event, RE::PlayerControls::Data024* a_data);
		static _ProcessButton orig_ProcessButton;


		void hook_ProcessButton(ButtonEvent* a_event, RE::PlayerControls::Data024* a_data)
		{
			using QuickLootRE::LootMenu;
			static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

			if (LootMenu::GetSingleton()) {
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
				LootMenu::ClearContainerRef();
				player->StartActivation();
			} else {
				(this->*orig_ProcessButton)(a_event, a_data);
			}
		}


		static void installHook()
		{
			RelocPtr<_ProcessButton> vtbl_ProcessButton(READY_WEAPON_HANDLER_VTBL_META + 0x28);
			orig_ProcessButton = *vtbl_ProcessButton;
			SafeWrite64(vtbl_ProcessButton.GetUIntPtr(), GetFnAddr(&hook_ProcessButton));
		}
	};


	ReadyWeaponHandlerEx::_ProcessButton ReadyWeaponHandlerEx::orig_ProcessButton;


	class PlayerCharacterEx : public PlayerCharacter
	{
	public:
		typedef void(PlayerCharacterEx::*_StartActivation)();


		void hook_StartActivation()
		{
			static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

			QuickLootRE::LootMenu* singleton = QuickLootRE::LootMenu::GetSingleton();
			singleton ? singleton->TakeItem() : player->StartActivation();
		}


		static void installHook()
		{
			RelocAddr<_StartActivation> call_StartActivation(PLAYER_CHARACTER_START_ACTIVATION_CALL);
			g_branchTrampoline.Write5Call(call_StartActivation.GetUIntPtr(), GetFnAddr(&hook_StartActivation));
		}
	};


	void installHooks()
	{
		FirstPersonStateHandler::installHook();
		ThirdPersonStateHandler::installHook();
		FavoritesHandler::installHook();
		ReadyWeaponHandlerEx::installHook();
		PlayerCharacterEx::installHook();
	}
}
