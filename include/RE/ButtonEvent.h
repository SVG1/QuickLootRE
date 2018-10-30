#pragma once

#include "skse64/GameInput.h"  // IDEvent, InputEvent


namespace RE
{
	class ButtonEvent :
		public IDEvent,
		public InputEvent
	{
	public:
		virtual					~ButtonEvent();
		virtual bool			IsIDEvent();
		virtual BSFixedString*	GetControlID();


		inline bool IsPressed() const	{ return pressure > 0; }
		inline bool IsDown() const		{ return (pressure > 0) && (timer == 0.0f); }
		inline bool IsUp() const		{ return (pressure == 0) && (timer > 0.0f); }


		// members
		UInt32			keyMask;	// 20
		UInt32			pad24;		// 24
		float			pressure;	// 28
		float			timer;		// 2C
	};
}
