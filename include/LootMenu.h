#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/CustomMenu.h"  // CustomMenuCreator, CustomMenu
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameMenus.h"  // IMenu, UIMessage
#include "skse64/GameTypes.h"  // SimpleLock
#include "skse64/ScaleformValue.h"  // GFxValue

#include <vector>  // vector


namespace QuickLootRE
{
	class LootMenuCreator
	{
	public:
		static IMenu* Create();

	private:
		LootMenuCreator();
	};


	class LootMenu : public IMenu
	{
	public:
		LootMenu(const char* a_swfPath);

		static LootMenu* GetSingleton();

		virtual UInt32 ProcessMessage(UIMessage* a_message) override;

		void OnMenuOpen();
		void OnMenuClose();

	private:
		static LootMenu* _singleton;
		static SimpleLock _lock;
	};


	extern TESObjectREFR* g_crosshairRef;
}