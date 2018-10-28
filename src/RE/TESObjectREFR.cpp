#include "RE/TESObjectREFR.h"

#include "skse64/GameExtraData.h"  // ExtraOwnership, ExtraTextDisplayData
#include "skse64/GameFormComponents.h"  // TESContainer, TESFullName
#include "skse64/GameForms.h"  // TESFaction
#include "skse64/GameObjects.h"  // TESNPC, TESObjectCONT, TESActorBase
#include "skse64/GameTypes.h"  // BSFixedString

#include <exception>  // exception

#include <sstream>  // TODO


namespace RE
{
	TESNPC* TESObjectREFR::GetActorOwner()
	{
		ExtraOwnership* exOwnership = static_cast<ExtraOwnership*>(extraData.GetByType(kExtraData_Ownership));
		if (exOwnership && exOwnership->owner && exOwnership->owner->formType == kFormType_Character) {
			return static_cast<TESNPC*>(exOwnership->owner);
		} else {
			return 0;
		}
	}


	TESContainer * TESObjectREFR::GetContainer()
	{
		TESContainer* container = 0;
		if (baseForm) {
			switch (baseForm->formType) {
			case kFormType_Container:
			{
				TESObjectCONT* cont = static_cast<TESObjectCONT*>(baseForm);
				container = cont ? &cont->container : 0;
				break;
			}
			case kFormType_NPC:
			{
				TESActorBase* actorBase = static_cast<TESActorBase*>(baseForm);
				container = actorBase ? &actorBase->container : 0;
				break;
			}
			}
		}
		return container;
	}


	const char* TESObjectREFR::GetFullName()
	{
		const char* result = 0;
		if (baseForm) {
			TESFullName* fullName = 0;
			try {
				fullName = DYNAMIC_CAST(baseForm, TESForm, TESFullName);
				if (fullName) {
					result = fullName->GetName();
				}
			} catch (std::exception& e) {
				_ERROR(e.what());
			}
		}
		return result;
	}


	TESFaction* TESObjectREFR::GetFactionOwner()
	{
		ExtraOwnership* exOwnership = static_cast<ExtraOwnership*>(extraData.GetByType(kExtraData_Ownership));
		if (exOwnership && exOwnership->owner && exOwnership->owner->formType == kFormType_Faction) {
			return static_cast<TESFaction*>(exOwnership->owner);
		} else {
			return 0;
		}
	}


	bool TESObjectREFR::SetDisplayName(const BSFixedString& name, bool force)
	{
		bool renamed = false;

		ExtraTextDisplayData* xTextData = reinterpret_cast<ExtraTextDisplayData*>(extraData.GetByType(kExtraData_TextDisplayData));
		if (xTextData) {
			bool inUse = (xTextData->message || xTextData->owner);
			if (inUse && force) {
				xTextData->message = nullptr;
				xTextData->owner = nullptr;
			}
			renamed = (!inUse || force);
			CALL_MEMBER_FN(xTextData, SetName_Internal)(name.c_str());
		} else {
			ExtraTextDisplayData* newTextData = ExtraTextDisplayData::Create();
			CALL_MEMBER_FN(newTextData, SetName_Internal)(name.c_str());
			extraData.Add(kExtraData_TextDisplayData, newTextData);
			renamed = true;
		}

		return renamed;
	}
}
