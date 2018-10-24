#pragma once

#include "skse64/GameEvents.h"  // EventResult, EventDispatcher, MenuOpenCloseEvent
#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // TESContainer, TESContainer::Entry
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <vector>  // vector


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


	void getInventoryList(BSExtraData* a_xData, TESContainer* a_container);


	extern CrosshairRefEventHandler g_crosshairRefEventHandler;
	extern MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
}