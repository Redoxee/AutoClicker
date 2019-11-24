#include "pch.h"
#include "Generator.h"

namespace AutoClicker
{
	long Generator::Price()
	{
		return this->Definition->BasePrice;
	}
}