#pragma once

#include "skse64/GameFormComponents.h"  // ActorProcessManager, MiddleProcess
#include "skse64/GameTypes.h"  // UnkFormArray

class TESForm;


namespace RE
{
	class Actor;


	class ActorProcessManager
	{
	public:
		enum EquippedHand : UInt32
		{
			kEquippedHand_Left = 0,
			kEquippedHand_Right = 1
		};


		enum Flag : UInt8
		{
			kFlag_None		= 0,
			kFlag_Unk01		= 1 << 0,
			kFlag_Unk02		= 1 << 1,
			kFlag_Unk03		= 1 << 2,
			kFlag_DrawHead	= 1 << 3,
			kFlag_Mobile	= 1 << 4,
			kFlag_Reset		= 1 << 5
		};


		void					SetEquipFlag(Flag flag)					{ CALL_MEMBER_FN(reinterpret_cast<::ActorProcessManager*>(this), SetEquipFlag)(flag); }
		void					UpdateEquipment(Actor* actor)			{ CALL_MEMBER_FN(reinterpret_cast<::ActorProcessManager*>(this), UpdateEquipment)(reinterpret_cast<::Actor*>(actor)); }
		void					SetDataFlag(float flag)					{ CALL_MEMBER_FN(reinterpret_cast<::ActorProcessManager*>(this), SetDataFlag)(flag); }
		void					UpdateEquipment_Hooked(Actor* actor)	{ reinterpret_cast<::ActorProcessManager*>(this)->UpdateEquipment_Hooked(reinterpret_cast<::Actor*>(actor)); }


		// members
		UInt64					unk00;					// 00
		MiddleProcess*			middleProcess;			// 08
		void*					unk10;					// 10
		MiddleProcess::Data58	unk18;					// 18
		float					unk48;					// 48 - init'd to -1
		UInt32					unk4C;					// 4C
		UInt64					unk50;					// 50
		UInt32					unk58;					// 58
		UInt32					pad5C;					// 5C
		UInt64					unk60;					// 60
		UInt64					unk68;					// 68
		UInt64					unk70;					// 70
		UInt64					unk78;					// 78
		UInt64					unk80;					// 80
		UInt64					unk88;					// 88
		UInt32					unk90;					// 90
		float					timeOfDeath;			// 94 - GetTimeDead = (GameDaysPassed*24) - timeOfDeath
		float					unk98;					// 98 - related with above
		UInt32					pad9C;					// 9C
		UnkFormArray			forms;					// A0  seen containing [left hand, equip slot, right hand, equip slot, right hand, equip slot)
		UInt64					unkB8[7];				// B8 2nd slot points back to this adress
		TESForm*				equippedObject[2];		// F0
		UInt8					unk100[0x136 - 0x100];	// 100
		UInt8					unk136;					// 136
		SInt8					unk137;					// 137 - init'd to 3
		UInt8					unk138;					// 138
		UInt8					unk139;					// 139
		UInt8					unk13A;					// 13A
		UInt8					unk13B;					// 13B
		UInt32					pad13C;					// 13C
	};
}
