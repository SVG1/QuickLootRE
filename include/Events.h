#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/GameEvents.h"  // EventResult, EventDispatcher, MenuOpenCloseEvent
#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESContainer
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <map>
#include <vector>  // vector

#include "Input.h"


namespace QuickLootRE
{
	class TESContainerVisitor
	{
	public:
		virtual bool Accept(TESContainer::Entry* a_entry);
	};


	class EntryDataListVisitor
	{
	public:
		virtual bool Accept(InventoryEntryData* a_entryData);
	};


	class CrosshairRefEventHandler : public BSTEventSink<SKSECrosshairRefEvent>
	{
	public:
		virtual	EventResult ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher) override;
	};


	class MenuOpenCloseEventHandler : public BSTEventSink<MenuOpenCloseEvent>
	{
	public:
		virtual EventResult ReceiveEvent(MenuOpenCloseEvent* a_event, EventDispatcher<MenuOpenCloseEvent>* a_dispatcher) override;
	};


	class TESContainerChangedEventHandler : public BSTEventSink<TESContainerChangedEvent>
	{
	public:
		virtual EventResult ReceiveEvent(TESContainerChangedEvent* a_event, EventDispatcher<TESContainerChangedEvent>* a_dispatcher) override;
	};


	void getInventoryList(BaseExtraList* a_xList, TESContainer* a_container);


	typedef SInt32 Count;
	static std::map<TESForm*, Count> defaultMap;

	extern CrosshairRefEventHandler g_crosshairRefEventHandler;
	extern MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
	extern TESContainerChangedEventHandler g_containerChangedEventHandler;
}
