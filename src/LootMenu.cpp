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
			Setup();
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
		if (_badRender) {
			_badRender = false;
		} else if (IsOpen()) {
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
				SetPlatform();
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
				SetPlatform();
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
				SetPlatform();
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
		SimpleLocker lock(&_lock);

		_selectedIndex = 0;
		_isOpen = true;
		static RE::MenuControls* mc = RE::MenuControls::GetSingleton();
		mc->RegisterHandler(this);
		DelayedUpdater::Register();
	}


	void LootMenu::OnMenuClose()
	{
		if (IsOpen()) {
			SimpleLocker lock(&_lock);

			_isOpen = false;
			static RE::MenuControls* mc = RE::MenuControls::GetSingleton();
			mc->RemoveHandler(this);
			CloseContainer();
		}
	}


	void LootMenu::TakeItem()
	{
		typedef RE::PlayerCharacter::EventType	EventType;
		typedef RE::TESObjectREFR::RemoveType	RemoveType;

		SimpleLocker lock(&_lock);

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

		DelayedUpdater::Register();
	}

	void LootMenu::ModSelectedIndex(SInt32 a_indexOffset)
	{
		if (IsOpen()) {
			SimpleLocker lock(&_lock);

			_selectedIndex += a_indexOffset;
			if (_selectedIndex < 0) {
				_selectedIndex = 0;
			} else if (_selectedIndex > g_invList.size() - 1) {
				_selectedIndex = g_invList.size() - 1;
			}
			SetSelectedIndex();
		}
	}


	void LootMenu::SetPlatform()
	{
		if (IsOpen()) {
			SimpleLocker lock(&_lock);

			LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
			new (dlgt)LootMenuUIDelegate(".SetPlatform", 2);

			dlgt->args[0].SetNumber(_platform);
			dlgt->args[0].SetBool(false);

			g_task->AddUITask(dlgt);
		}
	}


	void LootMenu::Setup()
	{
		if (IsOpen()) {
			SimpleLocker lock(&_lock);

			LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
			new (dlgt)LootMenuUIDelegate(".Setup", 4);

			RE::GFxMovieDef* def = view->GetMovieDef();

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

			dlgt->args[0].SetNumber(x);
			dlgt->args[1].SetNumber(y);
			dlgt->args[2].SetNumber(scale);
			dlgt->args[3].SetNumber(opacity);

			Settings::isApplied = true;

			g_task->AddUITask(dlgt);
		}
	}


	void LootMenu::OpenContainer()
	{
		if (IsOpen()) {
			SimpleLocker lock(&_lock);

			LootMenuUIDelegate* dlgt = 0;

			try {
				dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
				new (dlgt)LootMenuUIDelegate(".openContainer", 6);

				view->CreateArray(&dlgt->args[0]);
				if (!dlgt->args[0].objectInterface) {
					throw bad_gfx_value_interface();
				}

				for (SInt32 i = 0; i < g_invList.size() && i < Settings::itemLimit; ++i) {
					GFxValue item;
					dlgt->AddRef();
					view->CreateObject(&item);

					GFxValue text;
					dlgt->AddRef();
					text.SetString(g_invList[i].name());
					GFxValue count;
					dlgt->AddRef();
					count.SetNumber(g_invList[i].count());
					GFxValue value;
					dlgt->AddRef();
					value.SetNumber(g_invList[i].value());
					GFxValue weight;
					dlgt->AddRef();
					weight.SetNumber(g_invList[i].weight());
					GFxValue isStolen;
					dlgt->AddRef();
					isStolen.SetBool(g_invList[i].isStolen());
					GFxValue iconLabel;
					dlgt->AddRef();
					iconLabel.SetString(g_invList[i].icon());

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
			} catch (std::exception& e) {
				_ERROR(e.what());
				dlgt->Dispose();
			}
		}
	}


	void LootMenu::CloseContainer()
	{
		SimpleLocker lock(&_lock);

		LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
		new (dlgt)LootMenuUIDelegate(".closeContainer", 0);
		g_task->AddUITask(dlgt);
	}


	void LootMenu::SetSelectedIndex()
	{
		if (IsOpen()) {
			SimpleLocker lock(&_lock);

			LootMenuUIDelegate* dlgt = (LootMenuUIDelegate*)Heap_Allocate(sizeof(LootMenuUIDelegate));
			new (dlgt)LootMenuUIDelegate(".setSelectedIndex", 1);

			dlgt->args[0].SetNumber(_selectedIndex);

			g_task->AddUITask(dlgt);
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

		SimpleLocker lock(&_lock);

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


	LootMenuUIDelegate::LootMenuUIDelegate(std::string a_target, UInt32 a_numArgs) :
		_target(a_target),
		args(a_numArgs + 10),
		_refCount(0)
	{
		for (int i = 0; i < a_numArgs; ++i) {
			args.emplace_back();
			AddRef();
		}
		//args.shrink_to_fit();
	}


	void LootMenuUIDelegate::Run()
	{
		if (LootMenu::IsOpen()) {
			RE::GFxMovieView* view = LootMenu::_singleton->view;

			std::string name = "_root.Menu_mc" + _target;

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
			while (_refCount--) {
				LootMenu::_singleton->Release();
			}
			Heap_Free(this);
		}
	}


	void LootMenuUIDelegate::AddRef()
	{
		LootMenu::_singleton->AddRef();
		++_refCount;
	}


	void DelayedUpdater::Run()
	{
		if (LootMenu::IsOpen()) {
			if (Settings::disableIfEmpty && g_invList.empty()) {
				CALL_MEMBER_FN(UIManager::GetSingleton(), AddMessage)(&LootMenu::GetName(), UIMessage::kMessage_Close, 0);
				LootMenu::ClearContainerRef();
			} else {
				LootMenu::_singleton->OpenContainer();
			}
		}
	}


	void DelayedUpdater::Dispose()
	{}


	void DelayedUpdater::Register()
	{
		static DelayedUpdater singleton;
		g_task->AddTask(&singleton);
	}


	LootMenu*			LootMenu::_singleton = 0;
	SimpleLock			LootMenu::_lock;
	SInt32				LootMenu::_selectedIndex = 0;
	RE::TESObjectREFR*	LootMenu::_containerRef = 0;
	bool				LootMenu::_isOpen = false;
	LootMenu::Platform	LootMenu::_platform = kPlatform_PC;
	bool				LootMenu::_badRender = false;

	SKSETaskInterface* g_task = 0;
}
