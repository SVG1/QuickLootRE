#pragma once

#include "skse64/GameObjects.h"


namespace RE
{
	class TESObjectLIGH : public ::TESBoundAnimObject
	{
	public:
		enum { kTypeID = kFormType_Light };

		// parents
		::TESFullName				fullName;		// 030
		::TESModelTextureSwap		texSwap;		// 040
		::TESIcon					icon;			// 078
		::BGSMessageIcon			messageIcon;	// 088
		::TESWeightForm				weight;			// 0A0
		::TESValueForm				value;			// 0B0
		::BGSDestructibleObjectForm	destructible;	// 0C0
		::BGSEquipType				equipType;		// 0D0


		enum Flag
		{
			kFlag_Dynamic				= 1 << 0,
			kFlag_CanBeCarried			= 1 << 1,
			kFlag_Flicker				= 1 << 3,
			kFlag_Pulse					= 1 << 7,
			kFlag_ShadowSpotlight		= 1 << 10,
			kFlag_Hemisphere			= 1 << 11,
			kFlag_ShadowOmniDirectional	= 1 << 12,
			kFlag_PortalStrict			= 1 << 13
		};


		struct DataE0
		{
			struct Color
			{
				UInt8	red;	// 0
				UInt8	green;	// 1
				UInt8	blue;	// 2
				UInt8	pad3;	// 3
			};
			STATIC_ASSERT(offsetof(Color, red) == 0x0);
			STATIC_ASSERT(offsetof(Color, green) == 0x1);
			STATIC_ASSERT(offsetof(Color, blue) == 0x2);
			STATIC_ASSERT(sizeof(Color) == 0x4);


			struct FlickerEffect
			{
				UInt32	period;				// 0
				UInt32	intensityAmplitude;	// 4
				UInt32	movementAmplitude;	// 8
			};
			STATIC_ASSERT(offsetof(FlickerEffect, period) == 0x0);
			STATIC_ASSERT(offsetof(FlickerEffect, intensityAmplitude) == 0x4);
			STATIC_ASSERT(offsetof(FlickerEffect, movementAmplitude) == 0x8);
			STATIC_ASSERT(sizeof(FlickerEffect) == 0xC);


			SInt32			time;			// 00
			UInt32			radius;			// 04
			Color			color;			// 08
			Flag			flags;			// 0C
			UInt32			fallofExponent;	// 10
			float			FOV;			// 14 - 90
			float			nearClip;		// 18 - .001
			FlickerEffect	flickerEffect;	// 1C
		};
		STATIC_ASSERT(offsetof(DataE0, time) == 0x00);
		STATIC_ASSERT(offsetof(DataE0, radius) == 0x04);
		STATIC_ASSERT(offsetof(DataE0, color) == 0x08);
		STATIC_ASSERT(offsetof(DataE0, flags) == 0x0C);
		STATIC_ASSERT(offsetof(DataE0, fallofExponent) == 0x10);
		STATIC_ASSERT(offsetof(DataE0, FOV) == 0x14);
		STATIC_ASSERT(offsetof(DataE0, nearClip) == 0x18);
		STATIC_ASSERT(offsetof(DataE0, flickerEffect) == 0x1C);
		STATIC_ASSERT(sizeof(DataE0) == 0x28);


		struct Data118
		{
			UInt64	unk0;	// 00
			UInt32	unk4;	// 08
			UInt32	padC;	// 0C
			UInt64	unk8;	// 10
		};
		STATIC_ASSERT(sizeof(Data118) == 0x18);


		bool CanBeCarried() const
		{
			return (unkE0.flags & kFlag_CanBeCarried) != 0;
		}


		DataE0	unkE0;		// 0E0 - DATA
		float	fadeValue;	// 108 - FNAM
		UInt32	pad10C;		// 10C - SNAM
		UInt64	unk110;		// 110
		Data118	unk118;		// 118
	};
	STATIC_ASSERT(offsetof(TESObjectLIGH, fullName) == 0x030);
	STATIC_ASSERT(offsetof(TESObjectLIGH, texSwap) == 0x040);
	STATIC_ASSERT(offsetof(TESObjectLIGH, icon) == 0x078);
	STATIC_ASSERT(offsetof(TESObjectLIGH, messageIcon) == 0x088);
	STATIC_ASSERT(offsetof(TESObjectLIGH, weight) == 0x0A0);
	STATIC_ASSERT(offsetof(TESObjectLIGH, destructible) == 0x0C0);
	STATIC_ASSERT(offsetof(TESObjectLIGH, equipType) == 0x0D0);
	STATIC_ASSERT(offsetof(TESObjectLIGH, unkE0) == 0x0E0);
	STATIC_ASSERT(offsetof(TESObjectLIGH, fadeValue) == 0x108);
	STATIC_ASSERT(sizeof(TESObjectLIGH) == 0x130);
}
