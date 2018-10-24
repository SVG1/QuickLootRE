#include "Events.h"

#include "skse64/GameBSExtraData.h"  // BSExtraData
#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/GameExtraData.h"  // InventoryEntryData, ExtraContainerChanges
#include "skse64/GameFormComponents.h"  // TESContainer, TESContainer::Entry
#include "skse64/GameReferences.h"  // TESObjectREFR, TESContainer
#include "skse64/GameRTTI.h"  // DYNAMIC_CAST
#include "skse64/GameMenus.h"  // UIManager
#include "skse64/PapyrusEvents.h"  // SKSECrosshairRefEvent

#include <string>  // string

#include "InventoryList.h"  // g_invList
#include "LootMenu.h"  // LootMenu, g_crosshairRef


namespace QuickLootRE
{
	bool TESContainerVisitor::Accept(TESContainer::Entry* a_entry)
	{
		if (a_entry->form->formType != kFormType_LeveledItem) {
			g_invList.add(a_entry->form, a_entry->count);
		}
		return true;
	}


	bool EntryDataListVisitor::Accept(InventoryEntryData* a_entryData)
	{
		g_invList.add(a_entryData);
		return true;
	}


	EventResult CrosshairRefEventHandler::ReceiveEvent(SKSECrosshairRefEvent* a_event, EventDispatcher<SKSECrosshairRefEvent>* a_dispatcher)
	{
		if (a_event && a_event->crosshairRef && a_event->crosshairRef->baseForm) {
			g_crosshairRef = a_event->crosshairRef;
			TESContainer* container = DYNAMIC_CAST(a_event->crosshairRef->baseForm, TESForm, TESContainer);
			static BSFixedString name("LootMenu");
			if (container) {
				g_invList.clear();
				getInventoryList(a_event->crosshairRef->extraData.m_data, container);
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&name, UIMessage::kMessage_Open, 0);
			} else {
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&name, UIMessage::kMessage_Close, 0);
			}
		}
		return kEvent_Continue;
	}


	EventResult MenuOpenCloseEventHandler::ReceiveEvent(MenuOpenCloseEvent* a_event, EventDispatcher<MenuOpenCloseEvent>* a_dispatcher)
	{
		return kEvent_Continue;
		static BSFixedString name = "LootMenu";
		if (a_event->menuName == name && MenuManager::GetSingleton()->IsMenuOpen(&name)) {
			// TODO
		}
		return kEvent_Continue;
	}


	void getInventoryList(BSExtraData* a_xData, TESContainer* a_container)
	{
		TESContainerVisitor containerOp;
		a_container->Visit(containerOp);

		ExtraContainerChanges* xContainerChanges = 0;
		EntryDataListVisitor entryDataListOp;
		while (a_xData) {
			if (a_xData->GetType() == kExtraData_ContainerChanges) {
				xContainerChanges = static_cast<ExtraContainerChanges*>(a_xData);
				if (xContainerChanges->data && xContainerChanges->data->objList) {
					xContainerChanges->data->objList->Find(entryDataListOp);
				}
			}
			a_xData = a_xData->next;
		}
	}


	CrosshairRefEventHandler g_crosshairRefEventHandler;
	MenuOpenCloseEventHandler g_menuOpenCloseEventHandler;
}