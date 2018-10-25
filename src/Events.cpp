#include "Events.h"

#include "skse64/GameBSExtraData.h"  // BSExtraData
#include "skse64/GameEvents.h"  // EventResult, EventDispatcher
#include "skse64/GameExtraData.h"  // InventoryEntryData, ExtraContainerChanges
#include "skse64/GameFormComponents.h"  // TESContainer
#include "skse64/GameInput.h"  // InputEvent, InputStringHolder
#include "skse64/GameReferences.h"  // TESObjectREFR
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
		static BSFixedString name("LootMenu");
		if (a_event && a_event->crosshairRef && a_event->crosshairRef->baseForm) {
			g_crosshairRef = a_event->crosshairRef;
			TESContainer* container = DYNAMIC_CAST(a_event->crosshairRef->baseForm, TESForm, TESContainer);
			if (container) {
				g_invList.clear();
				getInventoryList(a_event->crosshairRef->extraData.m_data, container);
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&name, UIMessage::kMessage_Open, 0);
				return kEvent_Continue;
			}
		}
		if (LootMenu::GetSingleton()) {
			CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&name, UIMessage::kMessage_Close, 0);
		}
		return kEvent_Continue;
	}


	EventResult MenuOpenCloseEventHandler::ReceiveEvent(MenuOpenCloseEvent* a_event, EventDispatcher<MenuOpenCloseEvent>* a_dispatcher)
	{
		return kEvent_Continue;
		// TODO
	}


	EventResult InputEventHandler::ReceiveEvent(InputEvent** a_event, InputEventDispatcher* a_dispatcher)
	{
		if (LootMenu::GetSingleton() && a_event && *a_event) {
			ButtonEvent* buttonEvent = DYNAMIC_CAST((*a_event), InputEvent, ButtonEvent);
			if (buttonEvent) {
				switch (buttonEvent->deviceType) {
				case kDeviceType_Gamepad:
					switch (buttonEvent->keyMask) {
					case Gamepad::kGamepad_Up:
						LootMenu::ModSelectedIndex(-1);
						break;
					case Gamepad::kGamepad_Down:
						LootMenu::ModSelectedIndex(1);
						break;
					case Gamepad::kGamepad_A:
						//LootMenu::TakeItem();
						break;
					}
					break;
				case kDeviceType_Mouse:
					switch (buttonEvent->keyMask) {
					case Mouse::kMouse_WheelUp:
						LootMenu::ModSelectedIndex(-1);
						break;
					case Mouse::kMouse_WheelDown:
						LootMenu::ModSelectedIndex(1);
						break;
					}
					break;
				case kDeviceType_Keyboard:
				{
					InputStringHolder* holder = InputStringHolder::GetSingleton();
					if (*buttonEvent->GetControlID() == holder->zoomIn) {
						LootMenu::ModSelectedIndex(-1);
					} else if (*buttonEvent->GetControlID() == holder->zoomOut) {
						LootMenu::ModSelectedIndex(1);
					} else if (*buttonEvent->GetControlID() == holder->activate) {
						//LootMenu::TakeItem();
						break;
					}
					break;
				}
				}
			}
		}
		return kEvent_Continue;
	}


	EventResult TESContainerChangedEventHandler::ReceiveEvent(TESContainerChangedEvent* a_event, EventDispatcher<TESContainerChangedEvent>* a_dispatcher)
	{
		if (a_event && LootMenu::GetSingleton() && (a_event->fromFormId == g_crosshairRef->baseForm->formID || a_event->toFormId == g_crosshairRef->baseForm->formID)) {
			TESContainer* container = DYNAMIC_CAST(g_crosshairRef->baseForm, TESForm, TESContainer);
			if (container) {
				g_invList.clear();
				getInventoryList(g_crosshairRef->extraData.m_data, container);
				static BSFixedString name("LootMenu");
				LootMenu::Update();
			}
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
	InputEventHandler g_inputEventHandler;
	TESContainerChangedEventHandler g_containerChangedEventHandler;
}
