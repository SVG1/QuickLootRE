#include "Keywords.h"

#include "skse64/GameForms.h"  // BGSKeyword, LookupFormByID

#include <vector>  // vector


namespace QuickLootRE
{
	bool initalizeKeywords()
	{
		std::vector<BGSKeyword*> vec;

		// Armor
		vec.push_back(VendorItemClothing = static_cast<BGSKeyword*>(LookupFormByID(0x0008F95B)));
		vec.push_back(VendorItemJewelry = static_cast<BGSKeyword*>(LookupFormByID(0x0008F95A)));

		// Books
		vec.push_back(VendorItemRecipe = static_cast<BGSKeyword*>(LookupFormByID(0x000F5CB0)));
		vec.push_back(VendorItemSpellTome = static_cast<BGSKeyword*>(LookupFormByID(0x000937A5)));

		// Misc
		vec.push_back(VendorItemAnimalHide = static_cast<BGSKeyword*>(LookupFormByID(0x000914EA)));
		vec.push_back(VendorItemDaedricArtifact = static_cast<BGSKeyword*>(LookupFormByID(0x000917E8)));
		vec.push_back(VendorItemGem = static_cast<BGSKeyword*>(LookupFormByID(0x000914ED)));
		vec.push_back(VendorItemAnimalPart = static_cast<BGSKeyword*>(LookupFormByID(0x000914EB)));
		vec.push_back(VendorItemOreIngot = static_cast<BGSKeyword*>(LookupFormByID(0x000914EC)));
		vec.push_back(VendorItemClutter = static_cast<BGSKeyword*>(LookupFormByID(0x000914E9)));
		vec.push_back(VendorItemFireword = static_cast<BGSKeyword*>(LookupFormByID(0x000BECD7)));

		// Weapons
		vec.push_back(WeapTypeWarhammer = static_cast<BGSKeyword*>(LookupFormByID(0x0006D930)));

		for (auto& keyword : vec) {
			if (!keyword) {
				return false;
			}
		}
		return true;
	}


	// Armor
	BGSKeyword* VendorItemClothing = 0;
	BGSKeyword* VendorItemJewelry = 0;

	// Books
	BGSKeyword* VendorItemRecipe = 0;
	BGSKeyword* VendorItemSpellTome = 0;

	// Misc
	BGSKeyword* VendorItemAnimalHide = 0;
	BGSKeyword* VendorItemDaedricArtifact = 0;
	BGSKeyword* VendorItemGem = 0;
	BGSKeyword* VendorItemAnimalPart = 0;
	BGSKeyword* VendorItemOreIngot = 0;
	BGSKeyword* VendorItemClutter = 0;
	BGSKeyword* VendorItemFireword = 0;

	// Weapons
	BGSKeyword* WeapTypeWarhammer = 0;
}
