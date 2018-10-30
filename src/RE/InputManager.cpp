#include "RE/InputManager.h"

#include "common/ITypes.h"  // UInt32
#include "skse64/GameTypes.h"  // BSFixedString, tArray


#include <sstream>  // TODO

namespace RE
{
	UInt32 InputManager::GetMappedKey(const BSFixedString& name, InputDevice deviceType, Context contextIdx) const
	{
		tArray<InputContext::Mapping>* maps = 0;

		switch (deviceType) {
		case InputDevice::kInputDevice_Mouse:
			maps = &context[contextIdx]->mouseMap;
			break;
		case InputDevice::kInputDevice_Gamepad:
			maps = &context[contextIdx]->gamepadMap;
			break;
		case InputDevice::kInputDevice_Keyboard:
			maps = &context[contextIdx]->keyboardMap;
			break;
		}

		if (maps) {
			InputContext::Mapping mapping;
			for (int i = 0; i < maps->count; ++i) {
				if (maps->GetNthItem(i, mapping) && mapping.name == name) {
					std::stringstream ss;
					ss << maps << "\n" << &mapping;
					_DMESSAGE(ss.str().c_str());
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
		case InputDevice::kInputDevice_Mouse:
			maps = &context[contextIdx]->mouseMap;
			break;
		case InputDevice::kInputDevice_Gamepad:
			maps = &context[contextIdx]->gamepadMap;
			break;
		case InputDevice::kInputDevice_Keyboard:
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
