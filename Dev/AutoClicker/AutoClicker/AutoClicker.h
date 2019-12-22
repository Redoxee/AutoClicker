#pragma once

#include <iostream>
#include <vector>

#include "Upgrade.h"

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
		Upgrade* Upgrades;

		Data();
		Data(const std::vector<UpgradeDefinition>& upgradeDefinitions);
		~Data();

		void CopyTo(Data& c) const
		{
			c.Score = this->Score;
			c.PassiveSpeed = this->PassiveSpeed;
			c.TickCount = this->TickCount;
			c.TargetScore = this->TargetScore;
			c.ClickValue = this->ClickValue;

			if (this->NumberOfUpgrades != c.NumberOfUpgrades)
			{
				delete[] c.Upgrades;
				c.NumberOfUpgrades = this->NumberOfUpgrades;
				c.Upgrades = new Upgrade[c.NumberOfUpgrades];
			}
			
			for (int index = 0; index < this->NumberOfUpgrades; ++index)
			{
				c.Upgrades[index] = this->Upgrades[index];
			}
		}
	};

	class AutoClicker
	{
	public:
		AutoClicker();
		~AutoClicker();

		void Initialize(std::vector<UpgradeDefinition>& upgradeDefinition);

		void Tick();
		void Click();
		bool IsOver();
		bool BuyUpgrade(int index);

		void GetData(Data& d) const
		{
			this->data->CopyTo(d);
		}

		friend std::ostream& operator<<(std::ostream&, const AutoClicker* c);

	private:
		std::vector<UpgradeDefinition> upgradeDefinitions;

		Data* data = nullptr;
	};
}