#pragma once

#include "RE/Actor.h"


namespace RE
{
	class Character : public Actor
	{
	public:
		enum { kTypeID = kFormType_Character };
	};
	STATIC_ASSERT(sizeof(Character) == 0x2B0);
}
