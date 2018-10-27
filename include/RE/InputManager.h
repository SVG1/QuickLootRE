#pragma once

#include "common/ITypes.h"  // UInt8, UInt32, SInt32
#include "skse64/GameEvents.h"  // BSTEventSource
#include "skse64/GameInput.h"  // InputManager
#include "skse64/GameTypes.h"  // BSFixedString, tArray


namespace RE
{
	class InputManager
	{
	public:
		enum Context : UInt32
		{
			kContext_Gameplay = 0,
			kContext_MenuMode,
			kContext_Console,
			kContext_ItemMenu,
			kContext_Inventory,
			kContext_DebugText,
			kContext_Favorites,
			kContext_Map,
			kContext_Stats,
			kContext_Cursor,
			kContext_Book,
			kContext_DebugOverlay,
			kContext_Journal,
			kContext_TFCMode,
			kContext_MapDebug,
			kContext_Lockpicking,
			kContext_Favor,
			kContext_Count = 17,

			kContext_Invalid = 18
		};


		enum ControlState
		{
			kControlState_Looking	= 0x002,
			kControlState_Flying	= 0x040,
			kControlState_Sneaking	= 0x080,
			kControlState_Menu		= 0x100,
			kControlState_Movement	= 0x401
		};


		struct InputContext
		{
			struct Mapping
			{
				BSFixedString	name;		// 00 - User Event Name
				UInt16			buttonID;	// 08
				UInt16			modifier;	// 0A
				UInt32			sortIndex;	// 0C
				UInt32			flags;		// 10 - User Event Binary Flag
				UInt32			pad14;		// 14
			};
			STATIC_ASSERT(offsetof(Mapping, name) == 0x00);
			STATIC_ASSERT(offsetof(Mapping, buttonID) == 0x08);
			STATIC_ASSERT(offsetof(Mapping, modifier) == 0x0A);
			STATIC_ASSERT(offsetof(Mapping, sortIndex) == 0x0C);
			STATIC_ASSERT(offsetof(Mapping, flags) == 0x10);
			STATIC_ASSERT(sizeof(Mapping) == 0x18);


			tArray<Mapping>	keyboardMap;
			tArray<Mapping>	mouseMap;
			tArray<Mapping>	gamepadMap;
		};


		enum
		{
			kInvalid = 0xFFFFFFFF
		};


		enum InputDevice : UInt32
		{
			kInputDevice_Keyboard = 0,
			kInputDevice_Mouse,
			kInputDevice_Gamepad
		};

		static InputManager*	GetSingleton() { return reinterpret_cast<InputManager*>(::InputManager::GetSingleton()); }
		UInt8					AllowTextInput(bool allow) { return reinterpret_cast<::InputManager*>(this)->AllowTextInput(allow); }

		UInt32					GetMappedKey(const BSFixedString& name, InputDevice deviceType, Context contextIdx = kContext_Gameplay) const;
		const BSFixedString&	GetUserEventName(UInt32 buttonID, InputDevice deviceType, Context contextIdx = kContext_Gameplay) const;

		bool					IsLookingControlsEnabled() const	{ return (controlState & kControlState_Looking) == kControlState_Looking; }
		bool					IsFlyingControlsEnabled() const		{ return (controlState & kControlState_Flying) == kControlState_Flying; }
		bool					IsSneakingControlsEnabled() const	{ return (controlState & kControlState_Sneaking) == kControlState_Sneaking; }
		bool					IsMenuControlsEnabled() const		{ return (controlState & kControlState_Menu) == kControlState_Menu; }
		bool					IsMovementControlsEnabled() const	{ return (controlState & kControlState_Movement) == kControlState_Movement; }


		void*					unkPtr000;					// 000
		BSTEventSource<void*>	unk008;						// 008 - TODO: template type
		InputContext*			context[kContext_Count];	// 060
		tArray<void*>			unk0E8;						// 0E8
		tArray<void*>			unk100;						// 100
		SInt32					controlState;				// 118 - init'd to -1
		UInt32					unk11C;						// 11C - init'd to 0x80000000
		UInt8					allowTextInput;				// 120 - init'd to 0
		UInt8					unk121;						// 121 - init'd to 0
		UInt8					unk122;						// 122 - init'd to 0
		UInt8					pad[5];						// 123
	};
	STATIC_ASSERT(offsetof(InputManager, context) == 0x060);
	STATIC_ASSERT(offsetof(InputManager, allowTextInput) == 0x120);
	STATIC_ASSERT(sizeof(InputManager) == 0x128);


	UInt32 InputManager::GetMappedKey(const BSFixedString& name, InputDevice deviceType, Context contextIdx) const
	{
		tArray<InputContext::Mapping>* maps = 0;

		switch (deviceType) {
		case kInputDevice_Mouse:
			maps = &context[contextIdx]->mouseMap;
			break;
		case kInputDevice_Gamepad:
			maps = &context[contextIdx]->gamepadMap;
			break;
		case kInputDevice_Keyboard:
			maps = &context[contextIdx]->keyboardMap;
			break;
		}

		if (maps) {
			InputContext::Mapping mapping;
			for (int i = 0; i < maps->count; ++i) {
				if (maps->GetNthItem(i, mapping) && mapping.name == name) {
					return mapping.buttonID;
				}
			}
		}

		return kInvalid;
	}


	const BSFixedString& InputManager::GetUserEventName(UInt32 buttonID, InputDevice deviceType, Context contextIdx) const
	{
		tArray<InputContext::Mapping>* maps = 0;

		switch (deviceType) {
		case kInputDevice_Mouse:
			maps = &context[contextIdx]->mouseMap;
			break;
		case kInputDevice_Gamepad:
			maps = &context[contextIdx]->gamepadMap;
			break;
		case kInputDevice_Keyboard:
			maps = &context[contextIdx]->keyboardMap;
			break;
		}

		static BSFixedString none = "";

		if (maps) {
			InputContext::Mapping mapping;
			for (int i = 0; i < maps->count; ++i) {
				if (maps->GetNthItem(i, mapping) && mapping.buttonID == buttonID) {
					return mapping.name;
				}
			}
		}

		return none;
	}
}
