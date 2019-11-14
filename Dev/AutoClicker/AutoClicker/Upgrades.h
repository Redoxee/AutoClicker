#pragma once

#include "AutoClicker.h"

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
		string Name;
		string target;
		UpgradeType UprageType;
		OperationType OperationType;
		long BaseValue;
		long BasePrice;
	};
}