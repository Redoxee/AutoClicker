#pragma once

#include <iostream>
#include <collection.h>
#include "Upgrades.h"

namespace AutoClicker
{
	struct Data
	{
	public:
		long Score;
		long PassiveSpeed;
		long TickCount;
		long TargetScore;
		long ClickValue;

		long* UpgradeBought;

		Data();
		Data(int size);
		~Data();

		void CopyTo(Data& c) const
		{
			c.Score = this->Score;
			c.PassiveSpeed = this->PassiveSpeed;
			c.TickCount = this->TickCount;
			c.TargetScore = this->TargetScore;
			c.ClickValue = this->ClickValue;
		}
	};

	class AutoClicker
	{
	public:

		UpgradeDefinition* UpgradeDefinitions;
		int numberOfUpgrades;

		AutoClicker();
		~AutoClicker();

		void Initialize(UpgradeDefinition* upgradeDefinitions, int numberOfUpgrades);

		void Tick();
		void Click();
		bool IsOver();

		void GetData(Data& d) const
		{
			this->data.CopyTo(d);
		}

		friend std::ostream& operator<<(std::ostream&, const AutoClicker* c);
	private:
		Data data;
	};
}