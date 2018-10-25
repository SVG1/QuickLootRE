#pragma once

#include "skse64/GameForms.h"  // BGSKeyword


namespace QuickLootRE
{
	bool initalizeKeywords();


	// Armor
	extern BGSKeyword* VendorItemClothing;
	extern BGSKeyword* VendorItemJewelry;

	// Books
	extern BGSKeyword* VendorItemRecipe;
	extern BGSKeyword* VendorItemSpellTome;

	// Misc
	extern BGSKeyword* VendorItemAnimalHide;
	extern BGSKeyword* VendorItemDaedricArtifact;
	extern BGSKeyword* VendorItemGem;
	extern BGSKeyword* VendorItemAnimalPart;
	extern BGSKeyword* VendorItemOreIngot;
	extern BGSKeyword* VendorItemClutter;
	extern BGSKeyword* VendorItemFireword;

	// Weapons
	extern BGSKeyword* WeapTypeWarhammer;
}
