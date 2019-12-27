#include "pch.h"
#include "Upgrade.h"

namespace AutoClicker
{
	double Upgrade::ComputeNextPrice()
	{
		return this->Definition->PriceIncreaseStrategy.ComputeNextPrice(static_cast<double>(this->Definition->BasePrice), static_cast<double>(this->Price), static_cast<double>(this->InstanceBought));
	}

	double PriceIncreaseStrategy::ComputeNextPrice(double basePrice, double currentPrice, double instanceBought) const
	{
		double result = 0;
		if (instanceBought < 1)
		{
			return basePrice;
		}

		switch (this->Type)
		{
		case PriceIncreaseType::Flat:
		{
			result = currentPrice + this->Rate;
			break;
		}
		case PriceIncreaseType::Factor:
		{
			result = currentPrice * this->Rate;
			break;
		}
		case PriceIncreaseType::Exponential:
		{
			result = basePrice * pow(this->Rate, instanceBought);
			break;
		}
		default:
			break;
		}
		return result;
	}
}