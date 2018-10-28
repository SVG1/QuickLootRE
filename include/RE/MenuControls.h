#pragma once

#include "common/ITypes.h"  // UInt8
#include "skse64/GameEvents.h"  // BSTEventSink, MenuModeChangeEvent
#include "skse64/GameInput.h"  // InputEvent, MenuControls
#include "skse64/GameTypes.h"  // tArray
#include "skse64/PapyrusEvents.h"  // MenuModeChangeEvent

#include "Offsets.h"


namespace RE
{
	class MenuControls
	{
	public:
		// Parents
		BSTEventSink<InputEvent*>			inputEventSink;				// 00
		BSTEventSink<MenuModeChangeEvent>	menuModeChangeEventSink;	// 08
		void*								unk08;						// 10 - singleton


		virtual ~MenuControls();

		inline static MenuControls*	GetSingleton()								{ return reinterpret_cast<MenuControls*>(::MenuControls::GetSingleton()); }

		inline void					RegisterHandler(MenuEventHandler* handler)	{ CALL_MEMBER_FN(this, RegisterHandler_Impl)(handler); }
		inline void					RemoveHandler(MenuEventHandler* handler)	{ CALL_MEMBER_FN(this, RemoveHandler_Impl)(handler); }


		// members
		tArray<MenuEventHandler>	handlers;		// 18
		tArray<void*>				regBuffer;		// 30
		void*						unk48;			// 48
		void*						unk50;			// 50
		void*						unk58;			// 58
		void*						unk60;			// 60
		void*						unk68;			// 68
		void*						unk70;			// 70
		void*						unk78;			// 78
		bool						nowProcessing;	// 80
		UInt8						pad81;			// 81
		bool						remapMode;		// 82
		UInt8						pad83;			// 83

	private:
		MEMBER_FN_PREFIX(MenuControls);
		DEFINE_MEMBER_FN(RegisterHandler_Impl, void, MENU_CONTROLS_REGISTER_HANDLER_IMPL, MenuEventHandler* handler);
		DEFINE_MEMBER_FN(RemoveHandler_Impl, void, MENU_CONTROLS_REMOVE_HANDLER_IMPL, MenuEventHandler* handler);
	};
}
