#pragma once

#undef PlaySound

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

#include "RE/IMenu.h"  // RE::IMenu
#include "RE/MenuEventHandler.h"  // RE::MenuEventHandler
#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR

class GFxValue;

namespace RE
{
	class ButtonEvent;
}


namespace QuickLootRE
{
	class LootMenuCreator
	{
	public:
		static RE::IMenu* Create();

	private:
		LootMenuCreator();
	};


	class LootMenu :
		public RE::IMenu,
		public RE::MenuEventHandler
	{
		friend class DelayedUpdater;
		friend class LootMenuCreator;
		friend class LootMenuUIDelegate;
	private:
		enum Platform : UInt32
		{
			kPlatform_PC = 0,
			kPlatform_Other = 2
		};


	public:
		LootMenu(const char* a_swfPath);

		static LootMenu*			GetSingleton();
		static BSFixedString		GetName();
		static void					SetContainerRef(TESObjectREFR* a_ref);
		static void					SetContainerRef(RE::TESObjectREFR* a_ref);
		static RE::TESObjectREFR*	GetContainerRef();
		static void					ClearContainerRef();
		static bool					CanOpen(RE::TESObjectREFR* a_ref);
		static bool					IsOpen();

		// IMenu
		virtual UInt32				ProcessMessage(UIMessage* a_message) override;
		virtual void				Render() override;

		// MenuEventHandler
		virtual bool				CanProcess(InputEvent* a_event) override;
		virtual bool				ProcessButton(RE::ButtonEvent* a_event) override;

		void						OnMenuOpen();
		void						OnMenuClose();
		void						TakeItem();
		void						ModSelectedIndex(SInt32 a_indexOffset);

		// Scaleform
		void						SetPlatform();
		void						Setup();
		void						OpenContainer();
		void						CloseContainer();
		void						SetSelectedIndex();

	private:
		bool						SingleLootEnabled();
		void						PlayAnimation(const char* fromName, const char* toName);
		void						PlayAnimationOpen();
		void						PlayAnimationClose();
		void						PlaySound(TESForm* a_item);


		static LootMenu*			_singleton;
		static SimpleLock			_lock;
		static SInt32				_selectedIndex;
		static RE::TESObjectREFR*	_containerRef;
		static bool					_isOpen;
		static Platform				_platform;
		static bool					_badRender;
	};


	class LootMenuUIDelegate : public UIDelegate_v1
	{
	public:
		LootMenuUIDelegate(std::string a_target, UInt32 a_numArgs);

		virtual void			Run() override;
		virtual void			Dispose() override;
		void					AddRef();


		std::vector<GFxValue>	args;

	private:
		std::string				_target;
		SInt32					_refCount;
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
