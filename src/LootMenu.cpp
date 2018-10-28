#include "LootMenu.h"

#include "skse64/GameAPI.h"  // g_thePlayer
#include "skse64/GameBSExtraData.h"  // BSExtraData
#include "skse64/GameExtraData.h"  // ExtraContainerChanges
#include "skse64/GameInput.h"  // InputEvent, InputStringHolder
#include "skse64/GameMenus.h"  // IMenu
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/GameTypes.h"  // BSFixedString, SimpleLock, SimpleLocker
#include "skse64/PluginAPI.h"  // SKSETaskInterface
#include "skse64/ScaleformExtendedData.h"
#include "skse64/ScaleformLoader.h"  // GFxLoader
#include "skse64/ScaleformValue.h"  // GFxValue

#include <string>  // string

#include "HasActivateChoiceVisitor.h"  // HasActivateChoiceVisitor
#include "Hooks.h"  // RegisterMenuEventHandler, RemoveMenuEventHandler
#include "Input.h"  // InputDevice
#include "ItemData.h"  // ItemData
#include "InventoryList.h"  // g_invList

#include "RE/Actor.h"  // RE::Actor
#include "RE/Character.h"

#include "RE/BaseExtraList.h"  // RE::BaseExtraList
#include "RE/BGSEntryPointPerkEntry.h"  // RE::BGSEntryPointPerkEntry
#include "RE/ExtraContainerChanges.h"  // RE::ExtraContainerChanges::RE
#include "RE/InputManager.h"  // RE::InputManager
#include "RE/MenuControls.h"  // RE::MenuControls
#include "RE/MenuManager.h"  // RE::MenuManager
#include "RE/PlayerCharacter.h"  // RE::PlayerCharacter
#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR


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


	BSFixedString LootMenu::GetName()
	{
		return "LootMenu";
	}


	void LootMenu::SetContainerRef(TESObjectREFR* a_ref)
	{
		_containerRef = reinterpret_cast<RE::TESObjectREFR*>(a_ref);
	}


	void LootMenu::SetContainerRef(RE::TESObjectREFR* a_ref)
	{
		_containerRef = a_ref;
	}


	TESObjectREFR* LootMenu::GetContainerRef()
	{
		return reinterpret_cast<TESObjectREFR*>(_containerRef);
	}


	void LootMenu::ClearContainerRef()
	{
		_containerRef = 0;
	}


	bool LootMenu::CanOpen(RE::TESObjectREFR*& a_ref)
	{
		if (!a_ref || !a_ref->baseForm) {
			return false;
		}

		static RE::MenuManager* mm = RE::MenuManager::GetSingleton();
		if (mm && mm->numPauseGame && mm->numStopCrosshairUpdate > 0) {
			return false;
		}

		static RE::InputManager* mapping = RE::InputManager::GetSingleton();
		if (!mapping || !mapping->IsMovementControlsEnabled()) {
			return false;
		}

		static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);
		if (player->GetGrabbedRef() || player->GetActorInFavorState() || player->IsInKillMove()) {
			return false;
		}


		bool bAnimationDriven;
		static BSFixedString strAnimationDriven = "bAnimationDriven";
		if (player->animGraphHolder.GetAnimationVariableBool(strAnimationDriven, bAnimationDriven) && bAnimationDriven) {
			return false;
		}

		if (player->IsInCombat()) {
			return false;
		}

#if 0
		//disabled for testing
		if (ref->IsOffLimits()) {
			return false;
		}
#endif

		RE::TESObjectREFR* containerRef = 0;
		switch (a_ref->baseForm->formType) {
		case kFormType_Activator:
		{
			UInt32 refHandle;
			if (a_ref->extraData.GetAshPileRefHandle(refHandle) && refHandle != *g_invalidRefHandle) {
				RE::TESObjectREFR* refPtr;
				if (RE::TESObjectREFR::LookupByHandle(refHandle, refPtr)) {
					containerRef = refPtr;
				}
			}
			break;
		}
		case kFormType_Container:
			if (!a_ref->IsLocked()) {
				containerRef = a_ref;
			}
			break;
		case kFormType_NPC:
			if (a_ref->IsDead(true)) {
				containerRef = a_ref;
			}
			break;
		}

		if (!containerRef) {
			return false;
		}

		UInt32 numItems = containerRef->GetNumItems(false, false);

		if (numItems == 0) {
			return false;
		}

		if (player->CanProcessEntryPointPerkEntry(RE::BGSEntryPointPerkEntry::kEntryPoint_Activate)) {
			HasActivateChoiceVisitor visitor(player, a_ref);
			player->VisitEntryPointPerkEntries(RE::BGSEntryPointPerkEntry::kEntryPoint_Activate, visitor);
			if (visitor.GetResult()) {
				return false;
			}
		}

		return true;
	}


	UInt32 LootMenu::ProcessMessage(UIMessage* a_message)
	{
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
		if (view && a_event->eventType == InputEvent::kEventType_Button) {
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
				ModSelectedIndex(-1);
				break;
			case Gamepad::kGamepad_Down:
				ModSelectedIndex(1);
				break;
			}
			break;
		case kDeviceType_Mouse:
			switch (a_event->keyMask) {
			case Mouse::kMouse_WheelUp:
				ModSelectedIndex(-1);
				break;
			case Mouse::kMouse_WheelDown:
				ModSelectedIndex(1);
				break;
			}
			break;
		case kDeviceType_Keyboard:
		{
			static InputStringHolder* holder = InputStringHolder::GetSingleton();
			if (*a_event->GetControlID() == holder->zoomIn) {
				ModSelectedIndex(-1);
			} else if (*a_event->GetControlID() == holder->zoomOut) {
				ModSelectedIndex(1);
			}
			break;
		}
		}
		return true;
	}


	void LootMenu::OnMenuOpen()
	{
		if (view) {
			SimpleLocker lock(&_lock);

			_singleton = this;
			_selectedIndex = 0;
			static RE::MenuControls* mc = RE::MenuControls::GetSingleton();
			mc->RegisterHandler(this);
			OpenContainer();
		}
	}


	void LootMenu::OnMenuClose()
	{
		if (_singleton) {
			SimpleLocker lock(&_lock);

			_singleton = 0;
			static RE::MenuControls* mc = RE::MenuControls::GetSingleton();
			mc->RemoveHandler(this);
			CloseContainer();
		}
	}


	void LootMenu::TakeItem()
	{
		if (_singleton) {
			if (_containerRef && !g_invList.empty()) {
				ItemData item = g_invList[_selectedIndex];
				g_invList.erase(g_invList.begin() + _selectedIndex);

				UInt32 handle = 0;
				BaseExtraList* xList = 0;
				if (item.entryData()->extendDataList && item.entryData()->extendDataList->Count() > 0) {
					xList = item.entryData()->extendDataList->GetNthItem(0);
				}

				ExtraContainerChanges* xContainerChanges = static_cast<ExtraContainerChanges*>(_containerRef->extraData.GetByType(kExtraData_ContainerChanges));
				if (!xContainerChanges) {
					RE::BaseExtraList* xList = reinterpret_cast<RE::BaseExtraList*>(&_containerRef->extraData);
					RE::ExtraContainerChanges::Data* changes = new RE::ExtraContainerChanges::Data(reinterpret_cast<::TESObjectREFR*>(_containerRef));
					xList->SetInventoryChanges(changes);
					changes->InitContainer();
				}

				_containerRef->RemoveItem(&handle, item.form(), item.count(), RE::TESObjectREFR::RemoveType::kRemoveType_Take, xList, *g_thePlayer, 0, 0);

				OpenContainer();
			}
		}
	}


	void LootMenu::ModSelectedIndex(SInt32 a_indexOffset)
	{
		if (view) {
			_selectedIndex += a_indexOffset;
			if (_selectedIndex < 0) {
				_selectedIndex = 0;
			} else if (_selectedIndex > g_invList.size() - 1) {
				_selectedIndex = g_invList.size() - 1;
			}
			SetSelectedIndex();
		}
	}


	void LootMenu::OpenContainer()
	{
		if (view) {
			LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
			new (dlgt)LootMenuUIDelegate(".openContainer", 6);

			view->CreateArray(&dlgt->args[0]);
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
				view->CreateObject(&item);
				item.SetMember("text", &text);
				item.SetMember("count", &count);
				item.SetMember("value", &value);
				item.SetMember("weight", &weight);
				item.SetMember("isStolen", &isStolen);
				item.SetMember("iconLabel", &iconLabel);

				dlgt->args[0].PushBack(&item);
			}

			dlgt->args[1].SetNumber(_containerRef->formID);
			dlgt->args[2].SetString(_containerRef->GetReferenceName());
			dlgt->args[3].SetString("Take");
			dlgt->args[4].SetString("Search");
			dlgt->args[5].SetNumber(_selectedIndex);

			g_task->AddUITask(dlgt);
		}
	}


	void LootMenu::CloseContainer()
	{
		LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
		new (dlgt)LootMenuUIDelegate(".closeContainer", 0);
		g_task->AddUITask(dlgt);
	}


	void LootMenu::SetSelectedIndex()
	{
		if (view) {
			SimpleLocker lock(&_lock);

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


	void DelayedUpdater::Run()
	{
		LootMenu* lootMenu = LootMenu::GetSingleton();
		if (lootMenu) {
			lootMenu->OpenContainer();
		}
	}


	void DelayedUpdater::Dispose()
	{}


	void DelayedUpdater::Register()
	{
		static DelayedUpdater singleton;
		g_task->AddTask(&singleton);
	}


	LootMenu* LootMenu::_singleton = 0;
	SimpleLock LootMenu::_lock;
	SInt32 LootMenu::_selectedIndex = 0;
	RE::TESObjectREFR* LootMenu::_containerRef = 0;


	SKSETaskInterface* g_task = 0;
}
