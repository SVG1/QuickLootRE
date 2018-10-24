#include "ItemData.h"

#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameForms.h"  // TESForm


namespace QuickLootRE
{
	ItemData::ItemData(InventoryEntryData* a_entryData) :
		_entryData(a_entryData),
		_type(kType_None),
		_priority(0),
		_name(),
		_isStolen(false),
		_isEnchanted(false),
		_isQuestItem(false)
	{
		_type = getItemType();
		_name = CALL_MEMBER_FN(_entryData, GenerateName)();
	}


	ItemData::Type ItemData::getItemType()
	{
		ItemData::Type type = ItemData::kType_None;

		switch (_entryData->type->formType) {
		case kFormType_ScrollItem:
			type = ItemData::kType_DefaultScroll;
			break;
		case kFormType_Armor:
			type = GetItemTypeArmor(static_cast<TESObjectARMO*>(form));
			break;
		case kFormType_Book:
			type = GetItemTypeBook(static_cast<TESObjectBOOK*>(form));
			break;
		case kFormType_Ingredient:
			type = ItemData::kType_DefaultIngredient;
			break;
		case kFormType_Light:
			type = ItemData::kType_MiscTorch;
			break;
		case kFormType_Misc:
			type = GetItemTypeMisc(static_cast<TESObjectMISC*>(form));
			break;
		case kFormType_Weapon:
			type = GetItemTypeWeapon(static_cast<TESObjectWEAP*>(form));
			break;
		case kFormType_Ammo:
			type = (static_cast<TESAmmo*>(form)->isBolt()) ? ItemData::kType_WeaponBolt : ItemData::kType_WeaponArrow;
			break;
		case kFormType_Key:
			type = ItemData::kType_DefaultKey;
			break;
		case kFormType_Potion:
			type = GetItemTypePotion(static_cast<AlchemyItem*>(form));
			break;
		case kFormType_SoulGem:
			type = GetItemTypeSoulGem(static_cast<TESSoulGem*>(form));
			break;
		}

		return type;
	}
}