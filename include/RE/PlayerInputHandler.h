#pragma once

#include "RE/PlayerControls.h"  // PlayerControls::Data024

class ButtonEvent;
class InputEvent;
class MouseMoveEvent;
class ThumbstickEvent;


namespace RE
{
	class PlayerInputHandler
	{
		virtual	bool	CanProcess(InputEvent* evn) = 0;
		virtual	void	ProcessThumbstick(ThumbstickEvent* event, PlayerControls::Data024* arg2);
		virtual	void	ProcessMouseMove(MouseMoveEvent* event, PlayerControls::Data024* arg2);
		virtual	void	ProcessButton(ButtonEvent* event, PlayerControls::Data024* arg2);

		inline bool		IsEnabled() const			{ return enabled != 0; }
		inline void		Enable(bool bEnable = true)	{ enabled = (bEnable) ? 1 : 0; }

		// members
		UInt32			enabled;	// 08
	};
}
