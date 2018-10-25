#pragma once

#include "common/ITypes.h"  // UInt32
#include "skse64/GameBSExtraData.h"  // BaseExtraList

#include <vector>


namespace QuickLootRE
{
	class ExtendDataListVisitor
	{
	public:
		ExtendDataListVisitor(std::vector<UInt32> a_whiteList, std::vector<UInt32> a_blackList);
		~ExtendDataListVisitor();

		bool Accept(BaseExtraList* a_xList);
		bool found();

	private:
		bool checkWhiteList(BaseExtraList* a_xList);
		bool checkBlackList(BaseExtraList* a_xList);

		std::vector<UInt32> _whiteList;
		std::vector<UInt32> _blackList;
		bool _found;
	};
}
