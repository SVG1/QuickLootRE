#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/CustomMenu.h"  // CustomMenuCreator, CustomMenu
#include "skse64/GameEvents.h"  // BSTEventSink, TESContainerChangedEvent
#include "skse64/GameInput.h"  // InputEvent
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameMenus.h"  // IMenu, UIMessage
#include "skse64/gamethreads.h"  // TaskDelegate
#include "skse64/GameTypes.h"  // BSFixedString, SimpleLock
#include "skse64/Hooks_UI.h"  // UIDelegate_v1
#include "skse64/PluginAPI.h"  // SKSETaskInterface
#include "skse64/ScaleformValue.h"  // GFxValue

#include <vector>  // vector
#include <string>  // string

#include "RE/MenuEventHandler.h"  // RE::MenuEventHandler
#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR


namespace QuickLootRE
{
	class LootMenuCreator
	{
	public:
		static IMenu* Create();

	private:
		LootMenuCreator();
	};


	class LootMenu :
		public IMenu,
		public RE::MenuEventHandler
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
			kFlag_PauseGame				= 1 << 0,
			kFlag_DoNotDeleteOnClose	= 1 << 1,
			kFlag_ShowCursor			= 1 << 2,
			kFlag_Unk0008				= 1 << 3,
			kFlag_Modal					= 1 << 4,
			kFlag_StopDrawingWorld		= 1 << 5,
			kFlag_Open					= 1 << 6,
			kFlag_PreventGameLoad		= 1 << 7,
			kFlag_Unk0100				= 1 << 8,
			kFlag_HideOther				= 1 << 9,
			kFlag_Unk0400				= 1 << 10,
			kFlag_DoNotPreventGameSave	= 1 << 11,
			kFlag_Unk1000				= 1 << 12,
			kFlag_ItemMenu				= 1 << 13,
			kFlag_StopCrosshairUpdate	= 1 << 14,
			kFlag_Unk8000				= 1 << 15,
			kFlag_Unk10000				= 1 << 16  // ignore mouse cursor?
		};


	public:
		LootMenu(const char* a_swfPath);

		static LootMenu*			GetSingleton();
		static BSFixedString		GetName();
		static void					SetContainerRef(TESObjectREFR* a_ref);
		static void					SetContainerRef(RE::TESObjectREFR* a_ref);
		static RE::TESObjectREFR*	GetContainerRef();
		static void					ClearContainerRef();
		static bool					CanOpen(RE::TESObjectREFR*& a_ref);

		// IMenu
		virtual UInt32				ProcessMessage(UIMessage* a_message) override;

		// MenuEventHandler
		virtual bool				CanProcess(InputEvent* a_event) override;
		virtual bool				ProcessButton(ButtonEvent* a_event) override;

		void						OnMenuOpen();
		void						OnMenuClose();
		void						TakeItem();
		void						ModSelectedIndex(SInt32 a_indexOffset);

		// Scaleform
		void						OpenContainer();
		void						CloseContainer();
		void						SetSelectedIndex();

	private:

		static LootMenu*			_singleton;
		static SimpleLock			_lock;
		static SInt32				_selectedIndex;
		static RE::TESObjectREFR*	_containerRef;
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


	class DelayedUpdater : public TaskDelegate
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;

		static void	Register();
	};


	extern SKSETaskInterface* g_task;
}
