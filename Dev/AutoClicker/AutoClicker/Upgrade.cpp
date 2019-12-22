#include "pch.h"
#include "Upgrade.h"

namespace AutoClicker
{
	long Upgrade::Price()
	{
		return this->Definition->BasePrice;
	}
}