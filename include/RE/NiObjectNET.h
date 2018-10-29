#pragma once

#include "skse64/NiObjects.h"  // NiObject
#include "skse64/GameTypes.h"  // BSFixedString

class NiExtraData;
class NiTimeController;


namespace RE
{
	class NiObjectNET : public NiObject
	{
	public:
		inline void					AddExtraData(NiExtraData* extraData)	{ reinterpret_cast<::NiObjectNET*>(this)->AddExtraData(extraData); }
		inline bool					RemoveExtraData(NiExtraData* extraData)	{ return reinterpret_cast<::NiObjectNET*>(this)->RemoveExtraData(extraData); }
		inline SInt32				GetIndexOf(NiExtraData* extraData)		{ return reinterpret_cast<::NiObjectNET*>(this)->GetIndexOf(extraData); }
		inline NiExtraData*			GetExtraData(BSFixedString name)		{ return reinterpret_cast<::NiObjectNET*>(this)->GetExtraData(name); }

		inline NiTimeController*	GetController() const					{ return m_controller; }


		// members
		const char*					m_name;					// 10
		NiTimeController*			m_controller;			// 18 next pointer at +0x30
		NiExtraData**				m_extraData;			// 20 extra data
		UInt16						m_extraDataLen;			// 28 max valid entry
		UInt16						m_extraDataCapacity;	// 2A array len
		UInt32						pad2C;					// 2C
	};
}
