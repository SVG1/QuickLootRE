#pragma once

#include "common/ITypes.h"  // UInt8, UInt16, UInt32, UInt64
#include "skse64/GameEvents.h"  // BSTEventSink, MenuOpenCloseEvent, MenuModeChangeEvent, TESFurnitureEvent
#include "skse64/GameInput.h"  // InputEvent, PlayerInputHandler
#include "skse64/GameTypes.h"  // tArray


namespace RE
{
	class PlayerControls
	{
	public:
		// Parents
		BSTEventSink<InputEvent*>			inputEvent;				// 000
		BSTEventSink<MenuOpenCloseEvent*>	menuOpenCloseEvent;		// 008
		BSTEventSink<MenuModeChangeEvent*>	menuModeChangeEvent;	// 010
		BSTEventSink<TESFurnitureEvent*>	furnitureEvent;			// 018
		UInt32								unk020;					// 020 - singleton?


		struct Data024
		{
			float		movementX;	// 00 - Strafe Left=-1, Strafe Right=1
			float		movementY;	// 04 - Forward=1, Back=-1
			float		unk08;		// 08
			float		unk0C;		// 0C
			float		unk10;		// 10
			float		unk14;		// 14
			float		unk18;		// 18
			float		unk1C;		// 1C
			UInt32		unk20;		// 20
			UInt8		autoRun;	// 24
			UInt8		runMode;	// 25
			UInt8		unk26;		// 26
			UInt8		unk27;		// 27
			UInt16		unk28;		// 28
			UInt8		unk2A;		// 2A
			UInt8		unk2B;		// 2B
			bool		remapMod;	// 2C
			UInt8		pad2D[7];	// 2D
		};


		virtual	~PlayerControls();

		virtual UInt32			Unk_01();

		inline static PlayerControls*	GetSingleton()	{ return reinterpret_cast<PlayerControls*>(::PlayerControls::GetSingleton()); }
		inline PlayerControls*			ctor_Hook()		{ return reinterpret_cast<PlayerControls*>(reinterpret_cast<::PlayerControls*>(this)->ctor_Hook()); }
		inline PlayerControls*			ctor()			{ return reinterpret_cast<PlayerControls*>(CALL_MEMBER_FN(reinterpret_cast<::PlayerControls*>(this), ctor)()); }


		// members
		Data024						data024;			// 024
		tArray<PlayerInputHandler*>	handlers;			// 058
		tArray<void*>				unk070;				// 070
		tArray<void*>				unk088;				// 088
		UInt8						unk0A0[8];			// 0A0
		UInt64						unk0A8;				// 0A8
		float						unk0B0[8];			// 0B0
		UInt32						unk0D0[10];			// 0D0
		UInt8						unk0F8[8];			// 0F8
		float						unk100[20];			// 100
		tArray<void*>				unk150;				// 150
		UInt64						unk168;				// 168

		PlayerInputHandler*			movementHandler;	// 170
		PlayerInputHandler*			lookHandler;		// 178
		PlayerInputHandler*			sprintHandler;		// 180
		PlayerInputHandler*			readyWeaponHandler; // 188
		PlayerInputHandler*			autoMoveHandler;	// 190
		PlayerInputHandler*			toggleRunHandler;	// 198
		PlayerInputHandler*			activateHandler;	// 1A0
		PlayerInputHandler*			jumpHandler;		// 1A8
		PlayerInputHandler*			shoutHandler;		// 1B0
		PlayerInputHandler*			attackBlockHandler; // 1B8
		PlayerInputHandler*			runHandler;			// 1C0
		PlayerInputHandler*			sneakHandler;		// 1C8
		PlayerInputHandler*			togglePOVHandler;	// 1D0
	};
}
