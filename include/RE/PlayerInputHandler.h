#pragma once

#include "RE/PlayerControls.h"  // PlayerControls::Data024

class InputEvent;
class MouseMoveEvent;
class ThumbstickEvent;


namespace RE
{
	class ButtonEvent;


	class PlayerInputHandler
	{
		virtual	bool	CanProcess(InputEvent* a_event) = 0;
		virtual	void	ProcessThumbstick(ThumbstickEvent* a_event, PlayerControls::Data024* a_arg2);
		virtual	void	ProcessMouseMove(MouseMoveEvent* a_event, PlayerControls::Data024* a_arg2);
		virtual	void	ProcessButton(ButtonEvent* a_event, PlayerControls::Data024* a_arg2);

		inline bool		IsEnabled() const				{ return enabled != 0; }
		inline void		Enable(bool a_enable = true)	{ enabled = (a_enable) ? 1 : 0; }

		// members
		UInt32			enabled;	// 08
	};
}
