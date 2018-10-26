#include "LootMenu.h"

#include "skse64/GameAPI.h"  // g_thePlayer
#include "skse64/GameBSExtraData.h"  // BSExtraData
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameInput.h"  // InputEvent, InputStringHolder
#include "skse64/GameMenus.h"  // IMenu
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameTypes.h"  // BSFixedString
#include "skse64/PluginAPI.h"  // SKSETaskInterface
#include "skse64/ScaleformExtendedData.h"
#include "skse64/ScaleformLoader.h"  // GFxLoader
#include "skse64/ScaleformValue.h"  // GFxValue

#include <string>  // string

#include "Hooks.h"  // RegisterMenuEventHandler, RemoveMenuEventHandler
#include "Input.h"  // InputDevice
#include "ItemData.h"  // ItemData
#include "InventoryList.h"  // g_invList

#include "RE_BaseExtraList.h"  // RE::BaseExtraList
#include "RE_ExtraContainerChanges.h"  // RE::ExtraContainerChanges::RE
#include "RE_TESObjectREFR.h"  // RE::TESObjectREFR


namespace QuickLootRE
{
	IMenu* LootMenuCreator::Create()
	{
		void* p = ScaleformHeap_Allocate(sizeof(LootMenu));
		if (p) {
			IMenu* menu = new (p) LootMenu("LootMenu");
			return menu;
		} else {
			return 0;
		}
	}


	LootMenu::LootMenu(const char* a_swfPath)
	{
		if (CALL_MEMBER_FN(GFxLoader::GetSingleton(), LoadMovie)(this, &view, a_swfPath, kScaleModeType_ShowAll, 0.0)) {
			IMenu::flags = kFlag_DoNotDeleteOnClose | kFlag_DoNotPreventGameSave | kFlag_Unk10000;
			IMenu::unk0C = 0x02;  // menuDepth, set lower than fade menu (3)
		}
	}


	LootMenu* LootMenu::GetSingleton()
	{
		return _singleton;
	}


	void LootMenu::ModSelectedIndex(SInt32 a_indexOffset)
	{
		if (_singleton) {
			_selectedIndex += a_indexOffset;
			if (_selectedIndex < 0) {
				_selectedIndex = 0;
			} else if (_selectedIndex > g_invList.size() - 1) {
				_selectedIndex = g_invList.size() - 1;
			}
			_singleton->SetSelectedIndex();
		}
	}


	void LootMenu::Update()
	{
		if (_singleton) {
			LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
			new (dlgt)LootMenuUIDelegate(".openContainer", 6);

			_singleton->view->CreateArray(&dlgt->args[0]);
			for (auto& invItem : g_invList) {
				GFxValue text;
				text.SetString(invItem.name());
				GFxValue count;
				count.SetNumber(invItem.count());
				GFxValue value;
				value.SetNumber(invItem.value());
				GFxValue weight;
				weight.SetNumber(invItem.weight());
				GFxValue isStolen;
				isStolen.SetBool(invItem.isStolen());
				GFxValue iconLabel;
				iconLabel.SetString(invItem.icon());

				GFxValue item;
				_singleton->view->CreateObject(&item);
				item.SetMember("text", &text);
				item.SetMember("count", &count);
				item.SetMember("value", &value);
				item.SetMember("weight", &weight);
				item.SetMember("isStolen", &isStolen);
				item.SetMember("iconLabel", &iconLabel);

				dlgt->args[0].PushBack(&item);
			}

			dlgt->args[1].SetNumber(g_crosshairRef->formID);
			dlgt->args[2].SetString(CALL_MEMBER_FN(g_crosshairRef, GetReferenceName)());
			dlgt->args[3].SetString("Take");
			dlgt->args[4].SetString("Search");
			dlgt->args[5].SetNumber(_selectedIndex);

			g_task->AddUITask(dlgt);
		}
	}


	void LootMenu::TakeItem()
	{
		if (_singleton) {
			RE::TESObjectREFR* containerRef = reinterpret_cast<RE::TESObjectREFR*>(g_crosshairRef);
			if (containerRef && !g_invList.empty()) {
				ItemData item = g_invList[_selectedIndex];
				g_invList.erase(g_invList.begin() + _selectedIndex);

				UInt32 handle = 0;
				BaseExtraList* xList = 0;
				if (item.entryData()->extendDataList && item.entryData()->extendDataList->Count() > 0) {
					xList = item.entryData()->extendDataList->GetNthItem(0);
				}

				ExtraContainerChanges* xContainerChanges = static_cast<ExtraContainerChanges*>(g_crosshairRef->extraData.GetByType(kExtraData_ContainerChanges));
				if (!xContainerChanges) {
					RE::BaseExtraList* xList = reinterpret_cast<RE::BaseExtraList*>(&g_crosshairRef->extraData);
					RE::ExtraContainerChanges::Data* changes = new RE::ExtraContainerChanges::Data(g_crosshairRef);
					xList->SetInventoryChanges(changes);
					changes->InitContainer();
				}

				containerRef->RemoveItem(&handle, item.form(), item.count(), RE::TESObjectREFR::RemoveType::kRemoveType_Take, xList, *g_thePlayer, 0, 0);

				Update();
			}
		}
	}


	BSFixedString LootMenu::GetName()
	{
		return "LootMenu";
	}


	UInt32 LootMenu::ProcessMessage(UIMessage* a_message)
	{
		if (a_message->message == 6) {
			return 2;
		}
		switch (a_message->message) {
		case UIMessage::kMessage_Open:
			OnMenuOpen();
			break;
		case UIMessage::kMessage_Close:
			OnMenuClose();
			break;
		}
		return 2;
	}


	bool LootMenu::CanProcess(InputEvent* a_event)
	{
		if (_singleton && a_event->eventType == InputEvent::kEventType_Button) {
			ButtonEvent* button = static_cast<ButtonEvent*>(a_event);
			switch (a_event->deviceType) {
			case kInputDevice_Gamepad:
				return (button->keyMask == kGamepad_Up || button->keyMask == kGamepad_Down);
			case kInputDevice_Mouse:
				return (button->keyMask == kMouse_WheelDown || button->keyMask == kMouse_WheelUp);
			case kInputDevice_Keyboard:
				static InputStringHolder* holder = InputStringHolder::GetSingleton();
				return (*a_event->GetControlID() == holder->zoomIn || *a_event->GetControlID() == holder->zoomOut);
			}
		}
		return false;
	}


	bool LootMenu::ProcessButton(ButtonEvent* a_event)
	{
		switch (a_event->deviceType) {
		case kDeviceType_Gamepad:
			switch (a_event->keyMask) {
			case Gamepad::kGamepad_Up:
				LootMenu::ModSelectedIndex(-1);
				break;
			case Gamepad::kGamepad_Down:
				LootMenu::ModSelectedIndex(1);
				break;
			case Gamepad::kGamepad_X:
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
				g_crosshairRef = 0;
				startActivation(*g_thePlayer);
				break;
			}
			break;
		case kDeviceType_Mouse:
			switch (a_event->keyMask) {
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
			static InputStringHolder* holder = InputStringHolder::GetSingleton();
			if (*a_event->GetControlID() == holder->zoomIn) {
				LootMenu::ModSelectedIndex(-1);
			} else if (*a_event->GetControlID() == holder->zoomOut) {
				LootMenu::ModSelectedIndex(1);
			} else if (*a_event->GetControlID() == holder->activate) {
				//LootMenu::TakeItem();
				break;
			}
			break;
		}
		}
		return true;
	}


	void LootMenu::OnMenuOpen()
	{
		if (view) {
			_singleton = this;
			_selectedIndex = 0;
			static MenuControls* mc = MenuControls::GetSingleton();
			RegisterMenuEventHandler(mc, this);
			Update();
		}
	}


	void LootMenu::OnMenuClose()
	{
		if (_singleton) {
			_singleton = 0;
			static MenuControls* mc = MenuControls::GetSingleton();
			RemoveMenuEventHandler(mc, this);
			LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
			new (dlgt)LootMenuUIDelegate(".closeContainer", 0);
			g_task->AddUITask(dlgt);
		}
	}


	void LootMenu::SetSelectedIndex()
	{
		if (view) {
			LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
			new (dlgt)LootMenuUIDelegate(".setSelectedIndex", 1);

			dlgt->args[0].SetNumber(_selectedIndex);

			g_task->AddUITask(dlgt);
		}
	}


	LootMenuUIDelegate::LootMenuUIDelegate(std::string a_target, UInt32 a_numArgs) :
		target(a_target),
		args(a_numArgs)
	{
		for (int i = 0; i < a_numArgs; ++i) {
			args.emplace_back();
		}
	}


	void LootMenuUIDelegate::Run()
	{
		LootMenu* loot = LootMenu::GetSingleton();
		if (loot) {
			GFxMovieView* view = loot->view;

			std::string name = "_root.Menu_mc" + target;

			GFxValue* value = 0;
			if (args.size() > 0) {
				value = &args[0];
			}

			view->Invoke(name.c_str(), 0, value, args.size());
		}
	}


	void LootMenuUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	LootMenu* LootMenu::_singleton = 0;
	SInt32 LootMenu::_selectedIndex = 0;


	TESObjectREFR* g_crosshairRef = 0;
	SKSETaskInterface* g_task = 0;
}
