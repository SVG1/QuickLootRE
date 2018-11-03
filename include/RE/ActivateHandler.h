#pragma once

#include "RE/HeldStateHandler.h"  // RE::HeldStateHandler
#include "RE/PlayerControls.h"  // RE::PlayerControls::Data024

class InputEvent;


namespace RE
{
	class ButtonEvent;


	class ActivateHandler : public HeldStateHandler
	{
	public:
		virtual	bool	CanProcess(InputEvent* a_event) override;
		virtual	void	ProcessButton(ButtonEvent* a_event, PlayerControls::Data024* a_arg2) override;
	};
}
