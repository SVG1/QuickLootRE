#include "LootMenu.h"

#include "skse64/GameMenus.h"  // IMenu
#include "skse64/GameReferences.h"  // TESObjectREFR
#include "skse64/PapyrusUI.h"  // UIInvokeDelegate
#include "skse64/ScaleformExtendedData.h"
#include "skse64/ScaleformLoader.h"  // GFxLoader
#include "skse64/ScaleformValue.h"  // GFxValue

#include "InventoryList.h"  // g_invList


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
		if (CALL_MEMBER_FN(GFxLoader::GetSingleton(), LoadMovie)(this, &view, a_swfPath, 1, 0.0)) {
			IMenu::flags = 0x02 | 0x800 | 0x10000;
			IMenu::unk0C = 2;
		}
	}


	LootMenu* LootMenu::GetSingleton()
	{
		return _singleton;
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


	void LootMenu::OnMenuOpen()
	{
		if (view) {
			_singleton = this;

			UIInvokeDelegate dlgt("LootMenu", "_root.Menu_mc.openContainer");

			const int NUM_ARGS = 6;
			dlgt.args.reserve(NUM_ARGS);
			for (int i = 0; i < NUM_ARGS; ++i) {
				dlgt.args.emplace_back();
			}

			view->CreateArray(&dlgt.args[0]);
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

				dlgt.args[0].PushBack(&item);
			}

			dlgt.args[1].SetNumber(g_crosshairRef->formID);
			dlgt.args[2].SetString("Container");
			dlgt.args[3].SetString("Take");
			dlgt.args[4].SetString("Search");
			dlgt.args[5].SetNumber(0.0);

			dlgt.Run();
		}
	}


	void LootMenu::OnMenuClose()
	{
		if (_singleton) {
			_singleton = 0;
		}
	}


	LootMenu* LootMenu::_singleton = 0;
	SimpleLock LootMenu::_lock;


	TESObjectREFR* g_crosshairRef = 0;
}
