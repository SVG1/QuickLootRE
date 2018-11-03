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
#include "skse64/PapyrusUI.h"  // UIDelegate
#include "skse64/PluginAPI.h"  // SKSETaskInterface
#include "skse64/ScaleformValue.h"  // GFxValue

#include <vector>
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
		friend class LootMenuCreator;
		friend class SetPlatforUIDelegate;
		friend class SetupUIDelegate;
		friend class OpenContainerUIDelegate;
		friend class CloseContainerUIDelegate;
		friend class SetSelectedIndexUIDelegate;
	private:
		enum Platform : UInt32
		{
			kPlatform_PC = 0,
			kPlatform_Other = 2
		};

	public:
		enum Scaleform : UInt32
		{
			kScaleform_SetPlatform,
			kScaleform_Setup,
			kScaleform_OpenContainer,
			kScaleform_CloseContainer,
			kScaleform_SetSelectedIndex
		};


		LootMenu(const char* a_swfPath);
		virtual ~LootMenu();

		static LootMenu*			GetSingleton();
		static BSFixedString		GetName();

		static void					SetContainerRef(TESObjectREFR* a_ref);
		static void					SetContainerRef(RE::TESObjectREFR* a_ref);
		static RE::TESObjectREFR*	GetContainerRef();
		static void					ClearContainerRef();

		static bool					CanOpen(RE::TESObjectREFR* a_ref, bool a_isSneaking);
		static bool					IsOpen();

		static void					Register(Scaleform a_reg);

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

	private:
		bool						SingleLootEnabled();
		void						PlayAnimation(const char* fromName, const char* toName);
		void						PlayAnimationOpen();
		void						PlayAnimationClose();
		void						PlaySound(TESForm* a_item);


		static LootMenu*			_singleton;
		static SInt32				_selectedIndex;
		static RE::TESObjectREFR*	_containerRef;
		static bool					_isOpen;
		static Platform				_platform;
	};


	class SetPlatforUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class SetupUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class OpenContainerUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class CloseContainerUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class SetSelectedIndexUIDelegate : public UIDelegate_v1
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;
	};


	class GFxValueDeallocTaskDelegate : public TaskDelegate
	{
	public:
		virtual void Run() override;
		virtual void Dispose() override;


		std::vector<GFxValue*> heapAllocVals;
	};


	extern SKSETaskInterface* g_task;
}
