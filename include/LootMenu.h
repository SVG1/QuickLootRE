#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/CustomMenu.h"  // CustomMenuCreator, CustomMenu
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameMenus.h"  // IMenu, UIMessage
#include "skse64/GameTypes.h"  // SimpleLock
#include "skse64/Hooks_UI.h"  // UIDelegate_v1
#include "skse64/PluginAPI.h"  // SKSETaskInterface
#include "skse64/ScaleformValue.h"  // GFxValue

#include <vector>  // vector
#include <string>  // string


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
	private:
		enum ScaleModeType : UInt32
		{
			kScaleModeType_NoScale,
			kScaleModeType_ShowAll,
			kScaleModeType_ExactFit,
			kScaleModeType_NoBorder
		};


		enum Flag : UInt32
		{
			kFlag_PauseGame = 0x01,
			kFlag_DoNotDeleteOnClose = 0x02,
			kFlag_ShowCursor = 0x04,
			kFlag_Unk0008 = 0x08,
			kFlag_Modal = 0x10,
			kFlag_StopDrawingWorld = 0x20,
			kFlag_Open = 0x40,
			kFlag_PreventGameLoad = 0x80,
			kFlag_Unk0100 = 0x100,
			kFlag_HideOther = 0x200,
			kFlag_Unk0400 = 0x400,
			kFlag_DoNotPreventGameSave = 0x800,
			kFlag_Unk1000 = 0x1000,
			kFlag_ItemMenu = 0x2000,
			kFlag_StopCrosshairUpdate = 0x4000,
			kFlag_Unk8000 = 0x8000,
			kFlag_Unk10000 = 0x10000  // ignore mouse cursor?
		};


	public:
		LootMenu(const char* a_swfPath);

		static LootMenu* GetSingleton();
		static void ModSelectedIndex(SInt32 a_indexOffset);
		static void Update();
		static void TakeItem();

		virtual UInt32 ProcessMessage(UIMessage* a_message) override;

		void OnMenuOpen();
		void OnMenuClose();
		void SetSelectedIndex();

	private:
		static LootMenu* _singleton;
		static SInt32 _selectedIndex;
	};


	class LootMenuUIDelegate : public UIDelegate_v1
	{
	public:
		LootMenuUIDelegate(std::string a_target, UInt32 a_numArgs);

		virtual void Run() override;
		virtual void Dispose() override;

		std::vector<GFxValue> args;
		std::string target;
	};


	extern TESObjectREFR* g_crosshairRef;
	extern SKSETaskInterface* g_task;
}
