#pragma once

#include <iostream>
#include <vector>

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

		size_t NumberOfUpgrades;
		long* UpgradeBought = nullptr;
		long* UpgradePrices = nullptr;
		long* UpgradeBaseValues = nullptr;

		Data();
		Data(size_t size);
		~Data();

		void CopyTo(Data& c) const
		{
			c.Score = this->Score;
			c.PassiveSpeed = this->PassiveSpeed;
			c.TickCount = this->TickCount;
			c.TargetScore = this->TargetScore;
			c.ClickValue = this->ClickValue;

			if (c.UpgradeBought != nullptr)
			{
				delete(c.UpgradeBought);
				c.UpgradeBought = nullptr;
			}

			if (this->UpgradeBought != nullptr)
			{
				c.UpgradeBought = new long[this->NumberOfUpgrades];
				_memccpy(c.UpgradeBought, this->UpgradeBought, static_cast<int>(this->NumberOfUpgrades), sizeof(long));
			}

			c.NumberOfUpgrades = this->NumberOfUpgrades;
		}
	};

	class AutoClicker
	{
	public:

		std::vector<UpgradeDefinition> UpgradeDefinitions;
		int numberOfUpgrades;

		AutoClicker();
		~AutoClicker();

		void Initialize(std::vector<UpgradeDefinition>& upgradeDefinitions);

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