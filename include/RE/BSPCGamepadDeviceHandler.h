#pragma once

#include "RE/BSIInputDevice.h"  // RE::BSIInputDevice


namespace RE
{
	class BSGamepadDevice;


	class BSPCGamepadDeviceHandler : public BSIInputDevice
	{
	public:
		virtual ~BSPCGamepadDeviceHandler();


		// members
		BSGamepadDevice* gamepad;	// 08
	};
}
