#include "ServiceLocator.h"

namespace pal3
{
	ServiceLocator::~ServiceLocator()
	{
		for (auto it = _services.begin();it != _services.end();++it)
		{
			delete it->second;
		}
		_services.clear();
	}
}





