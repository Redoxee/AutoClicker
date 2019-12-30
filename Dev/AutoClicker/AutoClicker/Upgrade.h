#pragma once
#include <string>

namespace AutoClicker
{
	enum UpgradeType
	{
		Generator,
		ClickValue,
		Prestige,
		UpgradeImprove
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

		long BaseImpactValue;
		int TargetInfo;

		ValueIncreaseStrategy Impact;

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

		long CurrentImpactValue;
		
		double ComputeNextPrice();
	};
}