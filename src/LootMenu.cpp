#include "LootMenu.h"

#include "skse64/GameAPI.h"  // g_thePlayer
#include "skse64/GameInput.h"  // InputEvent, InputStringHolder
#include "skse64/GameMenus.h"  // IMenu
#include "skse64/GameTypes.h"  // BSFixedString, SimpleLock, SimpleLocker
#include "skse64/NiRTTI.h"  // ni_cast
#include "skse64/PluginAPI.h"  // SKSETaskInterface
#include "skse64/ScaleformLoader.h"  // GFxLoader
#include "skse64/ScaleformValue.h"  // GFxValue

#include <exception>  // exception
#include <string>  // string

#include "Exceptions.h"  // bad_gfx_value
#include "HasActivateChoiceVisitor.h"  // HasActivateChoiceVisitor
#include "Hooks.h"  // PlayAnimation(), PlaySound()
#include "ItemData.h"  // ItemData
#include "InventoryList.h"  // g_invList
#include "Settings.h"  // Settings

#include "RE/ActorProcessManager.h"  // RE::ActorProcessManager
#include "RE/BaseExtraList.h"  // RE::BaseExtraList
#include "RE/BGSEntryPointPerkEntry.h"  // RE::BGSEntryPointPerkEntry
#include "RE/BSWin32GamepadDevice.h"  // RE::BSWin32GamepadDevice
#include "RE/BSWin32KeyboardDevice.h"  // RE::BSWin32KeyboardDevice
#include "RE/BSWin32MouseDevice.h"  // RE::BSWin32MouseDevice
#include "RE/ButtonEvent.h"  // RE::ButtonEvent
#include "RE/ExtraContainerChanges.h"  // RE::ExtraContainerChanges, RE::ExtraContainerChanges::Data
#include "RE/GFxMovieDef.h"  // RE::GFxMovieDef
#include "RE/GFxLoader.h"  // RE::GFxLoader
#include "RE/GFxMovieView.h"  // RE::GFxMovieView
#include "RE/IMenu.h"  // RE::IMenu
#include "RE/InputEventDispatcher.h"  // RE::InputEventDispatcher
#include "RE/InputManager.h"  // RE::InputMappingManager
#include "RE/MenuControls.h"  // RE::MenuControls
#include "RE/MenuManager.h"  // RE::MenuManager
#include "RE/NiControllerManager.h"  // RE::NiControllerManager
#include "RE/NiNode.h"  // RE::NiNode
#include "RE/PlayerCharacter.h"  // RE::PlayerCharacter
#include "RE/TESBoundObject.h"  // RE::TESBoundObject
#include "RE/TESObjectREFR.h"  // RE::TESObjectREFR

class TESObjectREFR;


namespace QuickLootRE
{
	RE::IMenu* LootMenuCreator::Create()
	{
		void* p = ScaleformHeap_Allocate(sizeof(LootMenu));
		if (p) {
			LootMenu::_singleton = new (p) LootMenu(LootMenu::GetName());
			return LootMenu::_singleton;
		} else {
			return 0;
		}
	}


	LootMenu::LootMenu(const char* a_swfPath)
	{
		typedef RE::GFxMovieView::ScaleModeType ScaleModeType;

		RE::GFxLoader* loader = RE::GFxLoader::GetSingleton();
		if (loader->LoadMovie(this, view, a_swfPath, ScaleModeType::kScaleModeType_ShowAll, 0.0)) {
			flags = kFlag_DoNotDeleteOnClose | kFlag_DoNotPreventGameSave | kFlag_Unk10000;
			context = 0x02;  // menuDepth, set lower than fade menu (3)
		}
	}


	LootMenu* LootMenu::GetSingleton()
	{
		return _singleton;
	}


	BSFixedString LootMenu::GetName()
	{
		static BSFixedString name = "LootMenu";
		return name;
	}


	void LootMenu::SetContainerRef(TESObjectREFR* a_ref)
	{
		_containerRef = reinterpret_cast<RE::TESObjectREFR*>(a_ref);
	}


	void LootMenu::SetContainerRef(RE::TESObjectREFR* a_ref)
	{
		_containerRef = a_ref;
	}


	RE::TESObjectREFR* LootMenu::GetContainerRef()
	{
		return _containerRef;
	}


	void LootMenu::ClearContainerRef()
	{
		if (IsOpen()) {
			_singleton->PlayAnimationClose();
		}
		_containerRef = 0;
	}


	bool LootMenu::CanOpen(RE::TESObjectREFR* a_ref)
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

		if (Settings::disableInCombat && player->IsInCombat()) {
			return false;
		}

		if (Settings::disableTheft && a_ref->IsOffLimits()) {
			return false;
		}

		RE::TESObjectREFR* containerRef = 0;
		switch (a_ref->baseForm->formType) {
		case kFormType_Activator:
		{
			UInt32 refHandle = a_ref->extraData.GetAshPileRefHandle(refHandle);
			if (refHandle != *g_invalidRefHandle) {
				RE::TESObjectREFR* refPtr = 0;
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

		if (Settings::disableIfEmpty && numItems <= 0) {
			return false;
		}

#if 0
		// Disabled until I can understand this better
		if (player->CanProcessEntryPointPerkEntry(RE::BGSEntryPointPerkEntry::kEntryPoint_Activate)) {
			HasActivateChoiceVisitor visitor(player, a_ref);
			player->VisitEntryPointPerkEntries(RE::BGSEntryPointPerkEntry::kEntryPoint_Activate, visitor);
			if (visitor.GetResult()) {
				return false;
			}
		}
#endif

		_containerRef = containerRef;

		return true;
	}


	bool LootMenu::IsOpen()
	{
		return _isOpen;
	}


	UInt32 LootMenu::ProcessMessage(UIMessage* a_message)
	{
		if (!Settings::isApplied) {
			Register(kScaleform_Setup);
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


	void LootMenu::Render()
	{
		if (IsOpen()) {
			view->Render();
		}
	}


	bool LootMenu::CanProcess(InputEvent* a_event)
	{
		typedef RE::BSInputDevice::InputDevice		InputDevice;
		typedef RE::BSWin32GamepadDevice::Gamepad	Gamepad;
		typedef RE::BSWin32MouseDevice::Mouse		Mouse;

		if (IsOpen() && a_event->eventType == InputEvent::kEventType_Button) {
			ButtonEvent* button = static_cast<ButtonEvent*>(a_event);
			switch (a_event->deviceType) {
			case InputDevice::kInputDevice_Gamepad:
				return (button->keyMask == Gamepad::kGamepad_Up || button->keyMask == Gamepad::kGamepad_Down);
			case InputDevice::kInputDevice_Mouse:
				return (button->keyMask == Mouse::kMouse_WheelDown || button->keyMask == Mouse::kMouse_WheelUp);
			case InputDevice::kInputDevice_Keyboard:
				static InputStringHolder* holder = InputStringHolder::GetSingleton();
				return (*a_event->GetControlID() == holder->zoomIn || *a_event->GetControlID() == holder->zoomOut);
			}
		}
		return false;
	}


	bool LootMenu::ProcessButton(RE::ButtonEvent* a_event)
	{
		typedef RE::BSWin32GamepadDevice::Gamepad	Gamepad;
		typedef RE::BSWin32MouseDevice::Mouse		Mouse;

		if (!a_event->IsDown()) {
			return true;
		}

		switch (a_event->deviceType) {
		case kDeviceType_Gamepad:
			if (_platform != kPlatform_Other) {
				_platform = kPlatform_Other;
				Register(kScaleform_SetPlatform);
			}
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
			if (_platform != kPlatform_PC) {
				_platform = kPlatform_PC;
				Register(kScaleform_SetPlatform);
			}
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
			if (_platform != kPlatform_PC) {
				_platform = kPlatform_PC;
				Register(kScaleform_SetPlatform);
			}
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
		_selectedIndex = 0;
		_isOpen = true;
		Register(kScaleform_OpenContainer);
	}


	void LootMenu::OnMenuClose()
	{
		if (IsOpen()) {
			_isOpen = false;
			Register(kScaleform_CloseContainer);
		}
	}


	void LootMenu::TakeItem()
	{
		typedef RE::PlayerCharacter::EventType	EventType;
		typedef RE::TESObjectREFR::RemoveType	RemoveType;

		if (!IsOpen() || !_containerRef || g_invList.empty()) {
			return;
		}

		// Evaluate # of items to remove and update inv list accordingly
		ItemData item = g_invList[_selectedIndex];
		SInt32 numItems = item.count();
		if (numItems > 1 && SingleLootEnabled()) {
			numItems = 1;
			g_invList[_selectedIndex].reduceCount();
		} else {
			g_invList.erase(g_invList.begin() + _selectedIndex);
			ModSelectedIndex(0);
		}

		static RE::PlayerCharacter* player = reinterpret_cast<RE::PlayerCharacter*>(*g_thePlayer);

		// Containers don't have ExtraContainerChanges if the player hasn't opened them yet, so we must add them ourselves
		RE::ExtraContainerChanges* xContainerChanges = static_cast<RE::ExtraContainerChanges*>(_containerRef->extraData.GetByType(kExtraData_ContainerChanges));
		if (!xContainerChanges) {
			RE::BaseExtraList* xList = &_containerRef->extraData;
			RE::ExtraContainerChanges::Data* changes = new RE::ExtraContainerChanges::Data(_containerRef);
			xList->SetInventoryChanges(changes);
			changes->InitContainer();
		}

		// Locate item's extra list (if any)
		BaseExtraList* xList = 0;
		if (item.entryData()->extendDataList && item.entryData()->extendDataList->Count() > 0) {
			xList = item.entryData()->extendDataList->GetNthItem(0);
		}

		// Pickup dropped items
		if (xList && xList->HasType(kExtraData_ItemDropper)) {
			RE::TESObjectREFR* refItem = reinterpret_cast<RE::TESObjectREFR*>((UInt64)xList - 0x70);
			player->PickUpItem(refItem, 1, false, true);
		} else {
			RemoveType lootMode = RemoveType::kRemoveType_Take;

			if (_containerRef->IsDead(false)) {
				player->PlayPickupEvent(item.form(), _containerRef->GetOwner(), _containerRef, EventType::kEventType_DeadBody);
			} else {
				player->PlayPickupEvent(item.form(), _containerRef->GetOwner(), _containerRef, EventType::kEventType_Container);

				if (_containerRef->IsOffLimits()) {
					lootMode = RemoveType::kRemoveType_Steal;
				}
			}

			// Remove projectile 3D
			RE::TESBoundObject* bound = static_cast<RE::TESBoundObject*>(item.form());
			if (bound) {
				bound->OnRemovedFrom(_containerRef);
			}

			if (_containerRef->baseForm->formType == kFormType_Character) {

				// Dispell worn item enchantments
				RE::Actor* actor = static_cast<RE::Actor*>(_containerRef);
				if (actor->processManager) {
					actor->DispelWornItemEnchantments();
					actor->processManager->UpdateEquipment_Hooked(actor);
				}
			} else {
				if (_containerRef->IsOffLimits()) {
					UInt32 totalValue = item.value() * numItems;
					player->SendStealAlarm(_containerRef, 0, 0, totalValue, _containerRef->GetOwner(), true);
				}

				PlayAnimationOpen();
			}

			player->PlaySounds(item.form(), true, false);

			UInt32 droppedHandle = 0;
			_containerRef->RemoveItem(&droppedHandle, item.form(), numItems, lootMode, xList, player, 0, 0);
		}

		Register(kScaleform_OpenContainer);
	}

	void LootMenu::ModSelectedIndex(SInt32 a_indexOffset)
	{
		if (IsOpen()) {
			_selectedIndex += a_indexOffset;
			if (_selectedIndex < 0) {
				_selectedIndex = 0;
			} else if (_selectedIndex > g_invList.size() - 1) {
				_selectedIndex = g_invList.size() - 1;
			}
			Register(kScaleform_SetSelectedIndex);
		}
	}


	bool LootMenu::SingleLootEnabled()
	{
		typedef RE::BSKeyboardDevice			BSKeyboardDevice;
		typedef RE::BSWin32KeyboardDevice		BSWin32KeyboardDevice;
		typedef RE::BSGamepadDevice				BSGamepadDevice;
		typedef RE::BSWin32GamepadDevice		BSWin32GamepadDevice;
		typedef RE::BSInputDevice::InputDevice	InputDevice;

		static RE::InputEventDispatcher* inputDispatcher = RE::InputEventDispatcher::GetSingleton();
		static RE::InputManager* inputManager = RE::InputManager::GetSingleton();
		static InputStringHolder* holder = InputStringHolder::GetSingleton();

		if (Settings::disableSingleLoot) {
			return false;
		}

		RE::BSWin32KeyboardDevice* keyboard = DYNAMIC_CAST(inputDispatcher->keyboard, BSKeyboardDevice, BSWin32KeyboardDevice);
		if (keyboard && keyboard->IsEnabled()) {
			static UInt32 keyRun = inputManager->GetMappedKey(holder->sprint, InputDevice::kInputDevice_Keyboard);
			if (keyRun != RE::InputManager::kInvalid && keyboard->IsPressed(keyRun)) {
				return true;
			}
		}

		RE::BSGamepadDevice* gamepadHandle = inputDispatcher->gamepadHandler ? inputDispatcher->gamepadHandler->gamepad : 0;
		RE::BSWin32GamepadDevice* gamepad = DYNAMIC_CAST(gamepadHandle, BSGamepadDevice, BSWin32GamepadDevice);
		if (gamepad && gamepad->IsEnabled()) {
			static UInt32 keySprint = inputManager->GetMappedKey(holder->sprint, InputDevice::kInputDevice_Gamepad);
			if (keySprint != RE::InputManager::kInvalid && gamepad->IsPressed(keySprint)) {
				return true;
			}
		}

		return false;
	}


	void LootMenu::PlayAnimation(const char* fromName, const char* toName)
	{
		typedef RE::NiControllerManager NiControllerManager;

		RE::NiNode* niNode = _containerRef->GetNiNode();
		if (!niNode) {
			return;
		}

		NiTimeController* controller = niNode->GetController();
		if (!controller) {
			return;
		}

		RE::NiControllerManager* manager = ni_cast(controller, NiControllerManager);
		if (!manager) {
			return;
		}

		RE::NiControllerSequence* fromSeq = manager->GetSequenceByName(fromName);
		RE::NiControllerSequence* toSeq = manager->GetSequenceByName(toName);
		if (!fromSeq || !toSeq) {
			return;
		}

		(*Hooks::PlayAnimation)(_containerRef, manager, toSeq, fromSeq, false);
	}


	void LootMenu::PlayAnimationOpen()
	{
		if (_containerRef && !_isOpen) {
			PlayAnimation("Close", "Open");
			_isOpen = true;
			if (_containerRef->formType != kFormType_Character) {
				_containerRef->ActivateRefChildren(*g_thePlayer);
			}
		}
	}


	void LootMenu::PlayAnimationClose()
	{
		if (_containerRef && _isOpen) {
			PlayAnimation("Open", "Close");
			_isOpen = false;
		}
	}


	void LootMenu::PlaySound(TESForm* a_item)
	{
		BGSPickupPutdownSounds* sounds = DYNAMIC_CAST(a_item, TESForm, BGSPickupPutdownSounds);
		if (sounds && sounds->pickUp) {
			(*Hooks::PlaySound)(sounds->pickUp, false, &_containerRef->pos, (*g_thePlayer)->GetNiNode());
		}
	}


	void LootMenu::Register(Scaleform a_reg)
	{
		switch (a_reg) {
		case kScaleform_SetPlatform:
		{
			SetPlatforUIDelegate* dlgt = (SetPlatforUIDelegate*)Heap_Allocate(sizeof(SetPlatforUIDelegate));
			new (dlgt)SetPlatforUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_Setup:
		{
			SetupUIDelegate* dlgt = (SetupUIDelegate*)Heap_Allocate(sizeof(SetupUIDelegate));
			new (dlgt)SetupUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_OpenContainer:
		{
			OpenContainerUIDelegate* dlgt = (OpenContainerUIDelegate*)Heap_Allocate(sizeof(OpenContainerUIDelegate));
			new (dlgt)OpenContainerUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_CloseContainer:
		{
			CloseContainerUIDelegate* dlgt = (CloseContainerUIDelegate*)Heap_Allocate(sizeof(CloseContainerUIDelegate));
			new (dlgt)CloseContainerUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		case kScaleform_SetSelectedIndex:
		{
			SetSelectedIndexUIDelegate* dlgt = (SetSelectedIndexUIDelegate*)Heap_Allocate(sizeof(SetSelectedIndexUIDelegate));
			new (dlgt)SetSelectedIndexUIDelegate;
			g_task->AddUITask(dlgt);
			break;
		}
		default:
			_ERROR("[ERROR] Invalid registration (%i)", a_reg);
		}
	}


	LootMenu*			LootMenu::_singleton = 0;
	SInt32				LootMenu::_selectedIndex = 0;
	RE::TESObjectREFR*	LootMenu::_containerRef = 0;
	bool				LootMenu::_isOpen = false;
	LootMenu::Platform	LootMenu::_platform = kPlatform_PC;


	void SetPlatforUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			GFxValue args[2];

			args[0].SetNumber(LootMenu::_platform);
			args[0].SetBool(false);

			LootMenu::_singleton->view->Invoke("_root.Menu_mc.SetPlatform", 0, args, 2);
		}
	}


	void SetPlatforUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void SetupUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			RE::MenuControls* mc = RE::MenuControls::GetSingleton();
			mc->RegisterHandler(LootMenu::_singleton);

			GFxValue args[4];

			RE::GFxMovieDef* def = LootMenu::_singleton->view->GetMovieDef();

			double x = Settings::positionX;
			double y = Settings::positionY;
			double scale = Settings::scale;
			double opacity = Settings::opacity;

			x = (0 <= x && x <= 100) ? (x * def->GetWidth() * 0.01) : -1;
			y = (0 <= y && y <= 100) ? (y * def->GetHeight() * 0.01) : -1;
			if (scale >= 0) {
				if (scale < 25)
					scale = 25;
				else if (scale > 400)
					scale = 400;
			}
			if (opacity >= 0) {
				if (opacity > 100)
					opacity = 100;
			}

			args[0].SetNumber(x);
			args[1].SetNumber(y);
			args[2].SetNumber(scale);
			args[3].SetNumber(opacity);

			Settings::isApplied = true;

			LootMenu::_singleton->view->Invoke("_root.Menu_mc.Setup", 0, args, 4);
		}
	}


	void SetupUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void OpenContainerUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			try {
				GFxValue args[6];

				LootMenu::_singleton->view->CreateArray(&args[0]);
				if (!args[0].objectInterface) {
					throw bad_gfx_value_interface();
				}

				SInt32 size = (g_invList.size() < Settings::itemLimit) ? g_invList.size() : Settings::itemLimit;

				GFxValue* item = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (item)GFxValue[size];
				GFxValue* text = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (text)GFxValue[size];
				GFxValue* count = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (count)GFxValue[size];
				GFxValue* value = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (value)GFxValue[size];
				GFxValue* weight = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (weight)GFxValue[size];
				GFxValue* isStolen = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (isStolen)GFxValue[size];
				GFxValue* iconLabel = (GFxValue*)ScaleformHeap_Allocate(sizeof(GFxValue) * size);
				new (iconLabel)GFxValue[size];

				for (SInt32 i = 0; i < size; ++i) {
					LootMenu::_singleton->view->CreateObject(&item[i]);

					text[i].SetString(g_invList[i].name());
					count[i].SetNumber(g_invList[i].count());
					value[i].SetNumber(g_invList[i].value());
					weight[i].SetNumber(g_invList[i].weight());
					isStolen[i].SetBool(g_invList[i].isStolen());
					iconLabel[i].SetString(g_invList[i].icon());

					item[i].SetMember("text", &text[i]);
					item[i].SetMember("count", &count[i]);
					item[i].SetMember("value", &value[i]);
					item[i].SetMember("weight", &weight[i]);
					item[i].SetMember("isStolen", &isStolen[i]);
					item[i].SetMember("iconLabel", &iconLabel[i]);

					args[0].PushBack(&item[i]);
				}

				args[1].SetNumber(LootMenu::_containerRef->formID);
				args[2].SetString(LootMenu::_containerRef->GetReferenceName());
				args[3].SetString("Take");
				args[4].SetString("Search");
				args[5].SetNumber(LootMenu::_selectedIndex);

				LootMenu::_singleton->view->Invoke("_root.Menu_mc.openContainer", 0, args, 6);

				GFxValueDeallocTaskDelegate* dlgt = (GFxValueDeallocTaskDelegate*)Heap_Allocate(sizeof(GFxValueDeallocTaskDelegate));
				new (dlgt)GFxValueDeallocTaskDelegate;
				dlgt->heapAllocVals.push_back(item);
				dlgt->heapAllocVals.push_back(text);
				dlgt->heapAllocVals.push_back(count);
				dlgt->heapAllocVals.push_back(value);
				dlgt->heapAllocVals.push_back(weight);
				dlgt->heapAllocVals.push_back(isStolen);
				dlgt->heapAllocVals.push_back(iconLabel);
				g_task->AddTask(dlgt);
			} catch (std::exception& e) {
				_ERROR(e.what());
			}
		}
	}


	void OpenContainerUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void CloseContainerUIDelegate::Run()
	{
		LootMenu::_singleton->view->Invoke("_root.Menu_mc.closeContainer", 0, 0, 0);
	}


	void CloseContainerUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void SetSelectedIndexUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			GFxValue args[1];

			args[0].SetNumber(LootMenu::_selectedIndex);

			LootMenu::_singleton->view->Invoke("_root.Menu_mc.setSelectedIndex", 0, args, 1);
		}
	}


	void SetSelectedIndexUIDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	void GFxValueDeallocTaskDelegate::Run()
	{
		for (auto& val : heapAllocVals) {
			ScaleformHeap_Free(val);
		}
	}


	void GFxValueDeallocTaskDelegate::Dispose()
	{
		if (this) {
			Heap_Free(this);
		}
	}


	SKSETaskInterface* g_task = 0;
}
