#pragma once

#include "skse64/GameForms.h"  // BGSKeyword


namespace QuickLootRE
{
	enum KeywordFormID : UInt32
	{
		// Armor
		kKeywordFormID_VendorItemClothing			= 0x0008F95B,
		kKeywordFormID_VendorItemJewelry			= 0x0008F95A,

		// Books
		kKeywordFormID_VendorItemRecipe				= 0x000F5CB0,
		kKeywordFormID_VendorItemSpellTome			= 0x000937A5,

		// Misc
		kKeywordFormID_VendorItemAnimalHide			= 0x000914EA,
		kKeywordFormID_VendorItemDaedricArtifact	= 0x000917E8,
		kKeywordFormID_VendorItemGem				= 0x000914ED,
		kKeywordFormID_VendorItemAnimalPart			= 0x000914EB,
		kKeywordFormID_VendorItemOreIngot			= 0x000914EC,
		kKeywordFormID_VendorItemClutter			= 0x000914E9,
		kKeywordFormID_VendorItemFireword			= 0x000BECD7,

		// Weapons
		kKeywordFormID_WeapTypeWarhammer			= 0x0006D930,
	};


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
