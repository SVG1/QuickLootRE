#pragma once

#include "skse64/GameTypes.h"


namespace RE
{
	class IAnimationGraphManagerHolder
	{
	public:
		virtual ~IAnimationGraphManagerHolder();

		virtual UInt32	Unk_01(void);
		virtual UInt32	Unk_02(void);
		virtual UInt32	Unk_03(void);
		virtual UInt32	Unk_04(void);
		virtual UInt32	Unk_05(void);
		virtual UInt32	Unk_06(void);
		virtual UInt32	Unk_07(void);
		virtual UInt32	Unk_08(void);
		virtual UInt32	Unk_09(void);
		virtual UInt32	Unk_0A(void);
		virtual UInt32	Unk_0B(void);
		virtual UInt32	Unk_0C(void);
		virtual UInt32	Unk_0D(void);
		virtual UInt32	Unk_0E(void);
		virtual UInt32	Unk_0F(void);
		virtual bool	GetAnimationVariableFloat(const BSFixedString& variableName, float& out);
		virtual bool	GetAnimationVariableInt(const BSFixedString& variableName, SInt32& out);
		virtual bool	GetAnimationVariableBool(const BSFixedString& variableName, bool& out);
	};
}
