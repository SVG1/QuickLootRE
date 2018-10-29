#pragma once

#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64/NiObjects.h"  // NiObject


namespace RE
{
	class NiControllerSequence : public NiObject
	{
	public:
		virtual bool				Unk_21(UInt32 arg1, UInt32 arg2);

		inline const BSFixedString&	GetName() const { return m_name; }

		// members
		BSFixedString	m_name;	// 10
	};
}
