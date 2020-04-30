#include "pch.h"
#include "Upgrade.h"

namespace AutoClicker
{
	void Upgrade::ComputePrice()
	{
		this->Definition->PriceIncreaseStrategy.ComputeNextValue(this->Price, this->Definition->BasePrice, this->InstanceBought);
	}

	void ValueIncreaseStrategy::ComputeNextValue(int64_t& value, int64_t baseValue, int64_t instanceBought) const
	{
		if (instanceBought < 1)
		{
			return;
		}

		switch (this->Type)
		{
			case ValueIncreaseType::Flat:
			{
				double delta = this->Rate * static_cast<double>(instanceBought);
				value += delta;
				break;
			}
			case ValueIncreaseType::Factor:
			{
				double factor = this->Rate * static_cast<double>(instanceBought);
				value *= factor;
				break;
			}
			case ValueIncreaseType::Exponential:
			{
				value = baseValue * pow(this->Rate, instanceBought);
				break;
			}
			case ValueIncreaseType::Overwrite:
				value = this->Rate * instanceBought;
			default:
				break;
		}
	}
}