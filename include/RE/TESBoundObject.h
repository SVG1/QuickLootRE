#pragma once

#include "skse64/GameObjects.h"  // TESObject
namespace RE
{
	class TESObjectREFR;


	class TESBoundObject : public TESObject
	{
	public:
		struct Bound
		{
			UInt16	x;
			UInt16	y;
			UInt16	z;
		};


		virtual void	Unk_47(UInt32 arg);
		virtual UInt32	Unk_48(void);
		virtual UInt32	Unk_49(UInt32 arg);
		virtual bool	Unk_4A(void);
		virtual bool	Unk_4B(void* arg0, BSString* dst);
		virtual bool	Unk_4C(void* arg0, UInt8 arg1, UInt32 arg2, float arg3);
		virtual void	Unk_4D(UInt32 arg);
		virtual void	OnRemovedFrom(TESObjectREFR* ref);
		virtual void	Unk_4F(void);
		virtual void	Unk_50(void);
		virtual void	Unk_51(void);


		Bound	bounds;		// 20
		Bound	bounds2;	// 26  (not 28!)
	};
}
