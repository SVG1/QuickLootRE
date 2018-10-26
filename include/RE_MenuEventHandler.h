#pragma once

#include "common/ITypes.h"  // UInt8
#include "skse64/GameInput.h"  // InputEvent, KinectEvent, ThumbstickEvent, MouseMoveEvent, ButtonEvent
#include "skse64/GameTypes.h"  // BSIntrusiveRefCounted


namespace RE
{
	class MenuEventHandler : public ::BSIntrusiveRefCounted
	{
	public:
		MenuEventHandler() : unk08(0), unk09(0) {}
		virtual ~MenuEventHandler() {}

		virtual bool	CanProcess(::InputEvent* a_event) = 0;
		virtual bool	ProcessKinect(::KinectEvent* a_event) { return false; }
		virtual bool	ProcessThumbstick(::ThumbstickEvent* a_event) { return false; }
		virtual bool	ProcessMouseMove(::MouseMoveEvent* a_event) { return false; }
		virtual bool	ProcessButton(::ButtonEvent* a_event) { return false; }

		// members
		UInt8	unk08;		// 08
		UInt8	unk09;		// 09
		UInt8	pad0A[2];	// 0A
	};
}
