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

	enum FailureFlags
	{
		None = 0,
		NotEnoughMoney = 1,
		PurchaseLimitReached = 2,
		GameOver = 4,
		UnknownUpgrade = 8,
	};


	inline FailureFlags operator|(FailureFlags a, FailureFlags b)
	{
		return static_cast<FailureFlags>(static_cast<int>(a) | static_cast<int>(b));
	}

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

		int64_t BasePrice;

		int64_t BaseImpactValue;
		int TargetInfo;

		ValueIncreaseStrategy Impact;

		std::wstring Name;
		std::wstring Description;

		ValueIncreaseStrategy PriceIncreaseStrategy;
	};

	class Upgrade
	{
	public:
		const UpgradeDefinition* Definition = nullptr;
		int64_t InstanceBought = 0;
		int64_t Price;
		int64_t CurrentImpactValue;
		
		double ComputeNextPrice();
	};
}