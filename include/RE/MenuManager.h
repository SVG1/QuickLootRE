#pragma once

#include "common/ITypes.h"  // UInt8, UInt32, UInt64
#include "skse64/GameEvents.h"  // EventDispatcher, MenuOpenCloseEvent, MenuModeChangeEvent
#include "skse64/GameMenus.h"  // MenuTableItem, MenuManager, IMenu
#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64/ScaleformMovie.h"  // GFxMovieView


namespace RE
{
	class MenuManager
	{
	public:
		typedef tHashSet<MenuTableItem, BSFixedString> MenuTable;


		struct Unknown3
		{
			UInt64	unk000;		// 000 - New in SE. Not init'd?

			UInt32	freqLow;	// 008 (= Frequency.LowPart)
			UInt32	freqHigh;	// 00C (= Frequency.HighPart)

			UInt64	unk010;		// 010 (= 0)
			UInt32	unk018;		// 018 (= 0)
			UInt32	unk01C;		// 01C (= 0)
			UInt32	unk020;		// 020 (= 0)
			UInt32	unk024;		// 024 (= 0)
			float	unk028;		// 028 (= frequency related)

			UInt32	unk02C;		// 02C
			UInt32	unk030;		// 030

			UInt32	unk034;		// 034 (= 0)
			UInt16	unk038;		// 038
			UInt8	unk03A;		// 03A (= 0)
			UInt8	pad03B[5];	// 03B
		};


		static MenuManager*						GetSingleton(void)								{ return reinterpret_cast<MenuManager*>(::MenuManager::GetSingleton()); }

		EventDispatcher<MenuOpenCloseEvent>*	MenuOpenCloseEventDispatcher()					{ return reinterpret_cast<::MenuManager*>(this)->MenuOpenCloseEventDispatcher(); }

		bool									IsMenuOpen(BSFixedString* menuName)				{ return reinterpret_cast<::MenuManager*>(this)->IsMenuOpen(menuName); }
		IMenu*									GetMenu(BSFixedString* menuName)				{ return reinterpret_cast<::MenuManager*>(this)->GetMenu(menuName); }
		GFxMovieView*							GetMovieView(BSFixedString* menuName)			{ return reinterpret_cast<::MenuManager*>(this)->GetMovieView(menuName); }
		void									ShowMenus(bool show)							{ reinterpret_cast<::MenuManager*>(this)->ShowMenus(show); }
		bool									IsShowingMenus()								{ return reinterpret_cast<::MenuManager*>(this)->IsShowingMenus(); }

		typedef IMenu* (*CreatorFunc)(void);
		void									Register(const char* name, CreatorFunc creator)	{ reinterpret_cast<::MenuManager*>(this)->Register(name, creator); }


		UInt64									unk_000;						// 000

		EventDispatcher<MenuOpenCloseEvent>		menuOpenCloseEventDispatcher;	// 008
		EventDispatcher<MenuModeChangeEvent>	menuModeChangeEventDispatcher;	// 060
		EventDispatcher<void*>					unk_064;						// 0B8 - New in 1.6.87.0 - Kinect related?

		UnkArray								menuStack;						// 110
		MenuTable								menuTable;						// 128   (Entries ptr at 150)
		SimpleLock								menuTableLock;					// 158
		UInt32									numPauseGame;					// 160 (= 0) += 1 if (imenu->flags & 0x0001)
		UInt32									numItemMenu;					// 164 (= 0) += 1 if (imenu->flags & 0x2000)
		UInt32									numPreventGameLoad;				// 168 (= 0) += 1 if (imenu->flags & 0x0080)
		UInt32									numDoNotPreventSaveGame;		// 16C (= 0) += 1 if (imenu->flags & 0x0800)
		UInt32									numStopCrosshairUpdate;			// 170 (= 0) += 1 if (imenu->flags & 0x4000)
		UInt32									numFlag8000;					// 174 (= 0) += 1 if (imenu->flags & 0x8000)
		UInt32									numFlag20000;					// 178 (= 0)  = 1 if (imenu->flags & 0x20000)
		UInt8									numModal;						// 17C (= 0)  = 1 if (imenu->flags & 0x10)
		UInt8									pad17D[3];						// 17D
		Unknown3								unk180;							// 180
		bool									showMenus;						// 1C0 (= 0)
		bool									unk1C1;							// 1C1 (= 0)
		char									pad1C2[6];						// 1C2
	};
}
