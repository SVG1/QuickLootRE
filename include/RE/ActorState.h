#pragma once

#include "skse64/GameReferences.h"  // IMovementState


namespace RE
{
	class ActorState : public IMovementState
	{
	public:
		enum State : UInt32
		{
			kState_Running		= 1 << 6,
			kState_Walking		= 1 << 7,
			kState_Sprinting	= 1 << 8,
			kState_Sneaking		= 1 << 9,
			kState_Swimming		= 1 << 10,
			kState_Sit			= (0x0F << 0x0E),
			kState_Flying		= (0x07 << 0x12)
		};


		enum SitState : UInt32
		{
			kSitState_NotSitting	= 0,
			kSitState_WantsToSit	= 2,
			kSitState_Sitting		= 3,
			kSitState_WantsToStand	= 4
		};


		enum SleepState : UInt32
		{
			kSleepState_NotSleeping		= 0,
			kSleepState_WantsToSleep	= 2,
			kSleepState_Sleeping		= 3,
			kSleepState_WantsToWake		= 4
		};


		virtual ~ActorState();

		UInt32			GetSitState() const;
		UInt32			GetSleepState() const;
		inline UInt32	GetFlyingState() const	{ return (flags04 >> 0x12) & 0x07; }
		inline bool		IsBleedingOut() const	{ UInt32 state = (flags04 >> 0x15) & 0x0F; return (state == 7 || state == 8); }
		inline bool		IsWeaponDrawn() const	{ return (flags08 >> 5 & 7) >= 3; }
		inline bool		IsSneaking() const		{ return (flags04 & kState_Sneaking) != 0; }
		inline bool		IsSwimming() const		{ return (flags04 & kState_Swimming) != 0; }
		inline bool		IsSprinting() const		{ return (flags04 & kState_Sprinting) != 0; }
		inline bool		IsFlying() const		{ UInt32 flyingState = GetFlyingState(); return (flyingState != 0) && (flyingState != 5); }
		inline bool		IsUnconscious() const	{ return (flags04 & 0x01E00000) == 0x00600000; }


		UInt32	flags04;	// 08
		UInt32	flags08;	// 0C
	};
	STATIC_ASSERT(sizeof(ActorState) == 0x10);
}
