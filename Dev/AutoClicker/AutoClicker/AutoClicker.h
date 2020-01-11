#pragma once

#include <iostream>
#include <vector>

#include "Upgrade.h"

namespace AutoClicker
{
	struct ConfigurationData
	{
	public:
		int64_t Score;
		int64_t TargetScore;
		int64_t ClickValue;
		int64_t PassiveSpeed;
		int64_t GlobalFactor;
		std::vector<UpgradeDefinition> UpgradeDefinitions;
	};

	struct Data
	{
	public:
		int64_t Score;
		int64_t PassiveSpeed;
		int64_t FrameCount;
		int64_t TargetScore;
		int64_t ClickValue;
		int64_t GlobalFactor;

		size_t NumberOfUpgrades;
		Upgrade* Upgrades;

		Data();
		Data(const std::vector<UpgradeDefinition>& upgradeDefinitions);
		~Data();

		void CopyTo(Data& c) const
		{
			c.Score = this->Score;
			c.PassiveSpeed = this->PassiveSpeed;
			c.FrameCount = this->FrameCount;
			c.TargetScore = this->TargetScore;
			c.ClickValue = this->ClickValue;
			c.GlobalFactor = this->GlobalFactor;

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

		void Initialize(const ConfigurationData& configuration);

		void Update();
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

		void inline ComputeUpgradeEffects();
	};
}