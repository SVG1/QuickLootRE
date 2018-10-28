#include "ExtendDataListVisitor.h"

#include "skse64/GameBSExtraData.h"  // BaseExtraList


namespace QuickLootRE
{
	ExtendDataListVisitor::ExtendDataListVisitor(std::vector<UInt32> a_whiteList, std::vector<UInt32> a_blackList) :
		_whiteList(a_whiteList),
		_blackList(a_blackList)
	{}


	ExtendDataListVisitor::~ExtendDataListVisitor()
	{}


	bool ExtendDataListVisitor::Accept(BaseExtraList* a_xList)
	{
		if (a_xList && checkWhiteList(a_xList) && checkBlackList(a_xList)) {
			_found = true;
			return false;
		} else {
			return true;
		}
	}


	bool ExtendDataListVisitor::found()
	{
		return _found;
	}


	bool ExtendDataListVisitor::checkWhiteList(BaseExtraList* a_xList)
	{
		for (auto& xType : _whiteList) {
			if (!a_xList->HasType(xType)) {
				return false;
			}
		}
		return true;
	}


	bool ExtendDataListVisitor::checkBlackList(BaseExtraList* a_xList)
	{
		for (auto& xType : _blackList) {
			if (a_xList->HasType(xType)) {
				return false;
			}
		}
		return true;
	}
}
