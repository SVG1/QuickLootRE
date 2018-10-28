#pragma once

#include <vector>

namespace RE
{
	class BaseExtraList;
}


namespace QuickLootRE
{
	class ExtendDataListVisitor
	{
	public:
		ExtendDataListVisitor(std::vector<UInt32> a_whiteList, std::vector<UInt32> a_blackList);
		~ExtendDataListVisitor();

		bool Accept(RE::BaseExtraList* a_xList);
		bool found();

	private:
		bool checkWhiteList(RE::BaseExtraList* a_xList);
		bool checkBlackList(RE::BaseExtraList* a_xList);

		std::vector<UInt32> _whiteList;
		std::vector<UInt32> _blackList;
		bool _found;
	};
}
