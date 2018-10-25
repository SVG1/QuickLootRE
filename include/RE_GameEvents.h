#pragma once

#include "skse64/GameEvents.h"  // EventDispatcher, TESContainerChangedEvent


namespace RE
{
	class EventDispatcherList
	{
	public:
		EventDispatcher<void>								unk000;						//	00
		EventDispatcher<void>								unk058;						//  58  - sink offset 010
		EventDispatcher<void>								unk0B0;						//  B0  - sink offset 018
		EventDispatcher<void>								unk108;						//  108 - sink offset 020
		EventDispatcher<void>								unk160;						//  160 - sink offset 028
		EventDispatcher<void>								unk1B8;						//  1B8 - sink offset 030
		EventDispatcher<void>								unk210;						//  210 - sink offset 038
		EventDispatcher<void>								unk2C0;						//  2C0 - sink offset 040
		EventDispatcher<void>								unk318;						//  318 - sink offset 048
		EventDispatcher<TESContainerChangedEvent>			containerChangedDispatcher;	//  370 - sink offset 050
	};
}
