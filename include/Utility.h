#pragma once

#include "common/ITypes.h"  // UInt64

#include <string>  // string


namespace QuickLootRE
{
	std::string boolToString(bool a_bool);
	std::string numToHexString(UInt64 a_num, UInt64 a_bytes = 4);
}