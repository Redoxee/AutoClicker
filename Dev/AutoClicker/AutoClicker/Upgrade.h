#pragma once
#include <string>

namespace AutoClicker
{
	enum UpgradeType
	{
		Generator,
		ClickValue,
	};

	enum PriceIncreaseType
	{
		Flat,
		Factor,
		Exponential,

	};

	class PriceIncreaseStrategy
	{
	public:
		PriceIncreaseStrategy()
		{
			this->Type = PriceIncreaseType::Flat;
			this->Rate = 0;
		}

		PriceIncreaseStrategy(PriceIncreaseType type, double rate)
		{
			this->Type = type;
			this->Rate = rate;
		}

		PriceIncreaseType Type;
		double Rate;

		double ComputeNextPrice(double basePrice, double currentPrice, double instanceBought) const;
	};

	class UpgradeDefinition
	{
	public:
		UpgradeType UpgradeType;
		bool Unique;

		long BasePrice;
		long PriceRate;

		long ImpactValue;

		std::string Name;
		std::string Description;

		PriceIncreaseStrategy PriceIncreaseStrategy;
	};

	class Upgrade
	{
	public:
		const UpgradeDefinition* Definition = nullptr;
		long InstanceBought = 0;

		long Price;
		
		double ComputeNextPrice();
	};
}