#pragma once
#include <string>

namespace AutoClicker
{
	enum class UpgradeType
	{
		Generator,
		ClickValue,
		Prestige,
		UpgradeImprove,

		ClickTemporaryBoostFactor,
		ClickTemporaryBoostDuration,

		WakeDuration,
	};

	enum class ValueIncreaseType
	{
		Flat,
		Factor,
		Exponential,
		Overwrite,
	};

	enum class Comparer
	{
		Smaller,
		Greater,
	};

	enum class FailureFlags
	{
		None = 0,
		NotEnoughMoney = 1,
		PurchaseLimitReached = 2,
		GameOver = 4,
		UnknownUpgrade = 8,
		LockedByAnOtherPurchase = 16,
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

	class UpgradeLock
	{
	public :
		int LockIndex = -1;
		int64_t targetValue = -1;
		Comparer Comparer;
	};

	class UpgradeDefinition
	{
	public:
		UpgradeType UpgradeType = UpgradeType::ClickValue;
		bool Unique = false;

		int64_t BasePrice = 0;

		int64_t BaseImpactValue = 0;
		int TargetInfo = 0;

		ValueIncreaseStrategy Impact;

		std::wstring Name;
		std::wstring Description;

		UpgradeLock Lock;

		ValueIncreaseStrategy PriceIncreaseStrategy;
	};

	class Upgrade
	{
	public:
		const UpgradeDefinition* Definition = nullptr;
		int64_t InstanceBought = 0;
		int64_t Price = 0;
		int64_t CurrentImpactValue = 0;
		
		double ComputeNextPrice();
	};
}