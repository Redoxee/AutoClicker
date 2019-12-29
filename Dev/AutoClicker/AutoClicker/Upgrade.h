#pragma once
#include <string>

namespace AutoClicker
{
	enum UpgradeType
	{
		Generator,
		ClickValue,
		Prestige,
	};

	enum ValueIncreaseType
	{
		Flat,
		Factor,
		Exponential,
	};

	class ValueIncreaseStrategy
	{
	public:
		ValueIncreaseStrategy()
		{
			this->Type = ValueIncreaseType::Flat;
			this->Rate = 0;
		}

		ValueIncreaseStrategy(ValueIncreaseType type, double rate)
		{
			this->Type = type;
			this->Rate = rate;
		}

		ValueIncreaseType Type;
		double Rate;

		double ComputeNextValue(double baseValue, double currentValue, double instanceValue) const;
	};

	class UpgradeDefinition
	{
	public:
		UpgradeType UpgradeType;
		bool Unique;

		long BasePrice;

		long ImpactValue;

		std::string Name;
		std::string Description;

		ValueIncreaseStrategy PriceIncreaseStrategy;
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