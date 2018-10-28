#include "RE/ActorState.h"

#include "common/ITypes.h"  // UInt32


namespace RE
{
	UInt32 ActorState::GetSitState() const
	{
		UInt32 state = (flags04 >> 0x0E) & 0x0F;
		switch (state) {
		case 1:
		case 2:
			return kSitState_WantsToSit;
		case 3:
			return kSitState_Sitting;
		case 4:
			return kSitState_WantsToStand;
		default:
			return kSitState_NotSitting;
		}
	}


	UInt32 ActorState::GetSleepState() const
	{
		UInt32 state = (flags04 >> 0x0E) & 0x0F;
		switch (state) {
		case 5:
		case 6:
			return kSleepState_WantsToSleep;
		case 7:
			return kSleepState_Sleeping;
		case 8:
			return kSleepState_WantsToWake;
		default:
			return kSleepState_NotSleeping;
		}
	}
}
