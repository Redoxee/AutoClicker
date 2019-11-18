#pragma once
#include <string>

using namespace std;

namespace AutoClicker
{
	enum UpgradeType
	{
		PassiveSpeed,
		ClickSpeed,
		Price,
	};

	enum OperationType
	{
		Addition,
		Factor,
	};

	struct UpgradeDefinition
	{
	public:
		string Name;
		string Target;
		UpgradeType UprageType;
		OperationType OperationType;
		long BaseValue;
		long BasePrice;
	};
}