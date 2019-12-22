#pragma once
#include <string>

namespace AutoClicker
{
	enum UpgradeType
	{
		Generator,
		ClickValue,
	};

	class UpgradeDefinition
	{
	public:
		UpgradeType UpgradeType;
		bool Unique;

		long BasePrice;
		long PriceRate;

		long ImpactValue;

		std::string Description;
	};

	class Upgrade
	{
	public:
		const UpgradeDefinition* Definition = nullptr;
		long InstanceBought = 0;
		
		long Price();
	};
}