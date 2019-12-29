#include "pch.h"
#include "Upgrade.h"

namespace AutoClicker
{
	double Upgrade::ComputeNextPrice()
	{
		return this->Definition->PriceIncreaseStrategy.ComputeNextValue(static_cast<double>(this->Definition->BasePrice), static_cast<double>(this->Price), static_cast<double>(this->InstanceBought));
	}

	double ValueIncreaseStrategy::ComputeNextValue(double baseValue, double currentValue, double instanceBought) const
	{
		double result = 0;
		if (instanceBought < 1)
		{
			return baseValue;
		}

		switch (this->Type)
		{
		case ValueIncreaseType::Flat:
		{
			result = currentValue + this->Rate;
			break;
		}
		case ValueIncreaseType::Factor:
		{
			result = currentValue * this->Rate;
			break;
		}
		case ValueIncreaseType::Exponential:
		{
			result = baseValue * pow(this->Rate, instanceBought);
			break;
		}
		default:
			break;
		}
		return result;
	}
}