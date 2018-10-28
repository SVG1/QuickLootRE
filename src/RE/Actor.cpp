#include "RE/Actor.h"

#include "skse64/GameForms.h"  // TESRace
#include "skse64/GameObjects.h"  // TESNPC


namespace RE
{
	TESRace * Actor::GetRace()
	{
		TESRace* race = 0;

		TESNPC* actorBase = GetActorBase();
		if (actorBase) {
			race = actorBase->race.race;
		}

		return race;
	}


	bool Actor::IsSneaking()
	{
		if (!actorState.IsSneaking()) {
			return false;
		}

		if (actorState.IsSwimming()) {
			return false;
		}

		if (IsOnMount()) {
			return false;
		}

		return true;
	}
}
