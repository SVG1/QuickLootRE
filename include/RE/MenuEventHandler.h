#pragma once

#include "skse64/GameTypes.h"  // BSIntrusiveRefCounted

class ButtonEvent;
class InputEvent;
class KinectEvent;
class MouseMoveEvent;
class ThumbstickEvent;


namespace RE
{
	class MenuEventHandler : public BSIntrusiveRefCounted
	{
	public:
		MenuEventHandler() : unk08(0), unk09(0) {}
		virtual ~MenuEventHandler() {}

		virtual bool		CanProcess(InputEvent* a_event)				= 0;
		inline virtual bool	ProcessKinect(KinectEvent* a_event)			{ return false; }
		inline virtual bool	ProcessThumbstick(ThumbstickEvent* a_event)	{ return false; }
		inline virtual bool	ProcessMouseMove(MouseMoveEvent* a_event)	{ return false; }
		inline virtual bool	ProcessButton(ButtonEvent* a_event)			{ return false; }


		UInt8	unk08;		// 08
		UInt8	unk09;		// 09
		UInt8	pad0A[2];	// 0A
	};
}
