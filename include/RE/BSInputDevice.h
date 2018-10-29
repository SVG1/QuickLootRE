#pragma once

#include "skse64/GameTypes.h"  // BSFixedString

#include "RE/BSIInputDevice.h"  // RE::BSIInputDevice
#include "RE/BSTHashMap.h"  // RE::BSTHashMap


namespace RE
{
	class BSInputDevice : public BSIInputDevice
	{
	public:
		enum InputDevice : UInt32
		{
			kInputDevice_Keyboard = 0,
			kInputDevice_Mouse,
			kInputDevice_Gamepad
		};


		struct Data
		{
			BSFixedString	name;	// 00
			float			timer;	// 08
			UInt32			pad0C;	// 0C
		};


		virtual ~BSInputDevice();

		virtual void				Initialize() = 0;
		virtual	void				Process(float unk1) = 0;
		virtual	void				Unk_03() = 0;
		virtual	bool				Unk_04(UInt32 unk0, void *unk1);
		virtual bool				Unk_05(void *unk0);
		virtual bool				Unk_06(UInt32 unk0, UInt32 *unk1);
		virtual bool				IsEnabled(void) const;
		virtual void				Unk_08(void* unk1);

		inline bool IsKeyboard() const				{ return type == kInputDevice_Keyboard; }
		inline bool IsMouse() const					{ return type == kInputDevice_Mouse; }
		inline bool IsGamepad() const				{ return type == kInputDevice_Gamepad; }
		inline bool IsPressed(UInt32 keyCode) const	{ Data *data = 0; return (codeMap.GetAt(keyCode, data) && data->timer > 0.0f); }


		// members
		InputDevice					type;		// 08
		UInt32						pad0C;		// 0C
		BSTHashMap<UInt32, Data*>	codeMap;	// 10
	};
}
