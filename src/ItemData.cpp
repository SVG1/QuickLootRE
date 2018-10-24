#include "ItemData.h"

#include "skse64/GameExtraData.h"  // InventoryEntryData
#include "skse64/GameFormComponents.h"  // BGSBipedObjectForm
#include "skse64/GameForms.h"  // TESForm, BGSKeyword, LookupFormByID
#include "skse64/GameObjects.h"  // TESObjectARMO, TESObjectBOOK, TESObjectMISC

#include "RE_BGSBipedObjectForm.h"  // RE::BGSBipedObjectForm


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
		initType();
		_name = CALL_MEMBER_FN(_entryData, GenerateName)();
	}


	UInt32 ItemData::type()
	{
		return _type;
	}


	void ItemData::initType()
	{
		_type = kType_None;

		switch (_entryData->type->formType) {
		case kFormType_ScrollItem:
			_type = kType_DefaultScroll;
			break;
		case kFormType_Armor:
			_type = getTypeArmor(static_cast<TESObjectARMO*>(_entryData->type));
			break;
		case kFormType_Book:
			_type = getTypeBook(static_cast<TESObjectBOOK*>(_entryData->type));
			break;
		case kFormType_Ingredient:
			_type = kType_DefaultIngredient;
			break;
		case kFormType_Light:
			_type = kType_MiscTorch;
			break;
		case kFormType_Misc:
			_type = getTypeMisc(static_cast<TESObjectMISC*>(_entryData->type));
			break;
		case kFormType_Weapon:
			_type = GetItemTypeWeapon(static_cast<TESObjectWEAP*>(form));
			break;
		case kFormType_Ammo:
			_type = (static_cast<TESAmmo*>(form)->isBolt()) ? kType_WeaponBolt : kType_WeaponArrow;
			break;
		case kFormType_Key:
			_type = kType_DefaultKey;
			break;
		case kFormType_Potion:
			_type = GetItemTypePotion(static_cast<AlchemyItem*>(form));
			break;
		case kFormType_SoulGem:
			_type = GetItemTypeSoulGem(static_cast<TESSoulGem*>(form));
			break;
		}
	}


	ItemData::Type ItemData::getTypeArmor(TESObjectARMO* a_armor)
	{
		static ItemData::Type types[] = {
		kType_LightArmorBody,		// 0
		kType_LightArmorHead,
		kType_LightArmorHands,
		kType_LightArmorForearms,
		kType_LightArmorFeet,
		kType_LightArmorCalves,
		kType_LightArmorShield,
		kType_LightArmorMask,

		kType_ArmorBody,			// 8
		kType_ArmorHead,
		kType_ArmorHands,
		kType_ArmorForearms,
		kType_ArmorFeet,
		kType_ArmorCalves,
		kType_ArmorShield,
		kType_ArmorMask,

		kType_ClothingBody,			// 16
		kType_ClothingHead,
		kType_ClothingHands,
		kType_ClothingForearms,
		kType_ClothingFeet,
		kType_ClothingCalves,
		kType_ClothingShield,
		kType_ClothingMask,

		kType_ArmorAmulet,			// 24
		kType_ArmorRing,
		kType_Circlet,

		kType_DefaultArmor			// 27
		};

		UInt32 index = 0;
		RE::BGSBipedObjectForm* bipedObj = reinterpret_cast<RE::BGSBipedObjectForm*>(&a_armor->bipedObject);

		if (bipedObj->IsLightArmor()) {
			index = 0;
		} else if (bipedObj->IsHeavyArmor()) {
			index = 8;
		} else {
			static BGSKeyword* VendorItemClothing = static_cast<BGSKeyword*>(LookupFormByID(0x08F95B));
			static BGSKeyword* VendorItemJewelry = static_cast<BGSKeyword*>(LookupFormByID(0x08F95A));

			if (a_armor->keyword.HasKeyword(VendorItemClothing)) {
				index = 16;
			} else if (a_armor->keyword.HasKeyword(VendorItemJewelry)) {
				if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Amulet)) {
					index = 24;
				} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Ring)) {
					index = 25;
				} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Circlet)) {
					index = 26;
				} else {
					index = 27;
				}
			} else {
				index = 27;
			}
		}

		if (index >= 24) {
			return types[index];
		}

		if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Body | RE::BGSBipedObjectForm::kPart_Tail)) {
			index += 0;			// body
		} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Head | RE::BGSBipedObjectForm::kPart_Hair | RE::BGSBipedObjectForm::kPart_LongHair)) {
			index += 1;			// head
			if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Circlet)) {
				index += 6;		// mask
			}
		} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Hands)) {
			index += 2;			// hands
		} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Forearms)) {
			index += 3;			// forearms
		} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Feet)) {
			index += 4;			// feet
		} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Calves)) {
			index += 5;			// calves
		} else if (bipedObj->HasPartOf(RE::BGSBipedObjectForm::kPart_Shield)) {
			index += 6;			// shield
		} else {
			index = 27;
		}

		return types[index];
	}


	ItemData::Type ItemData::getTypeBook(TESObjectBOOK* a_book)
	{
		Type type = kType_DefaultBook;

		static BGSKeyword* VendorItemRecipe = static_cast<BGSKeyword*>(LookupFormByID(0x000F5CB0));
		static BGSKeyword* VendorItemSpellTome = static_cast<BGSKeyword*>(LookupFormByID(0x000937A5));

		if (a_book->data.type == 0xFF || a_book->keyword.HasKeyword(VendorItemRecipe)) {
			type = kType_BookNote;
		} else if (a_book->keyword.HasKeyword(VendorItemSpellTome)) {
			type = kType_BookTome;
		}

		return type;
	}


	ItemData::Type ItemData::getTypeMisc(TESObjectMISC* a_misc)
	{
		ItemData::Type type = ItemData::kType_DefaultMisc;

		static const UInt32 LockPick = 0x00000A;
		static const UInt32 Gold = 0x00000F;
		static const UInt32 Leather01 = 0x000DB5D2;
		static const UInt32 LeatherStrips = 0x000800E4;

		static BGSKeyword* VendorItemAnimalHide = static_cast<BGSKeyword*>(LookupFormByID(0x0914EA));
		static const UInt32 VendorItemDaedricArtifact = 0x000917E8;
		static const UInt32 VendorItemGem = 0x000914ED;
		static const UInt32 VendorItemTool = 0x000914EE;
		static const UInt32 VendorItemAnimalPart = 0x000914EB;
		static const UInt32 VendorItemOreIngot = 0x000914EC;
		static const UInt32 VendorItemClutter = 0x000914E9;
		static const UInt32 VendorItemFireword = 0x000BECD7;

		static const UInt32 RubyDragonClaw = 0x04B56C;
		static const UInt32 IvoryDragonClaw = 0x0AB7BB;
		static const UInt32 GlassCraw = 0x07C260;
		static const UInt32 EbonyCraw = 0x05AF48;
		static const UInt32 EmeraldDragonClaw = 0x0ED417;
		static const UInt32 DiamondClaw = 0x0AB375;
		static const UInt32 IronClaw = 0x08CDFA;
		static const UInt32 CoralDragonClaw = 0x0B634C;
		static const UInt32 E3GoldenClaw = 0x0999E7;
		static const UInt32 SapphireDragonClaw = 0x0663D7;
		static const UInt32 MS13GoldenClaw = 0x039647;

		if (a_misc->formID == LockPick)
			type = ItemData::kType_MiscLockPick;
		else if (a_misc->formID == Gold)
			type = ItemData::kType_MiscGold;
		else if (a_misc->formID == Leather01)
			type = ItemData::kType_MiscLeather;
		else if (a_misc->formID == LeatherStrips)
			type = ItemData::kType_MiscStrips;
		else if (a_misc->keyword.HasKeyword(VendorItemAnimalHide))
			type = ItemData::kType_MiscHide;
		else if (a_misc->keyword.HasKeyword(VendorItemDaedricArtifact))
			type = ItemData::kType_MiscArtifact;
		else if (a_misc->keyword.HasKeyword(VendorItemGem))
			type = ItemData::kType_MiscGem;
		else if (a_misc->keyword.HasKeyword(VendorItemAnimalPart))
			type = ItemData::kType_MiscRemains;
		else if (a_misc->keyword.HasKeyword(VendorItemOreIngot))
			type = ItemData::kType_MiscIngot;
		else if (a_misc->keyword.HasKeyword(VendorItemClutter))
			type = ItemData::kType_MiscClutter;
		else if (a_misc->keyword.HasKeyword(VendorItemFireword))
			type = ItemData::kType_MiscWood;
		else if (a_misc->formID == RubyDragonClaw
			|| misc->formID == IvoryDragonClaw
			|| misc->formID == GlassCraw
			|| misc->formID == EbonyCraw
			|| misc->formID == EmeraldDragonClaw
			|| misc->formID == DiamondClaw
			|| misc->formID == IronClaw
			|| misc->formID == CoralDragonClaw
			|| misc->formID == E3GoldenClaw
			|| misc->formID == SapphireDragonClaw
			|| misc->formID == MS13GoldenClaw
			)
			type = ItemData::kType_MiscDragonClaw;

		return type;
	}
}