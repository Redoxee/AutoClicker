#pragma once

#include <iostream>
#include <vector>

#include "Upgrade.h"

namespace AutoClicker
{
	struct ConfigurationData
	{
	public:
		int64_t Score = 0;
		int64_t TargetScore = 0;
		int64_t ClickValue = 0;
		int64_t PassiveSpeed = 0;
		int64_t GlobalFactor = 0;
		int64_t TempBoostDuration = 0;
		int64_t WakeDuration = 0;
		std::vector<UpgradeDefinition> UpgradeDefinitions;
	};

	struct Data
	{
	public:
		int64_t Score = 0;
		int64_t PassiveSpeed = 0;
		int64_t FrameCount = 0;
		int64_t TargetScore = 0;
		int64_t ClickValue = 0;
		int64_t GlobalFactor = 0;

		int64_t ClickTemporaryBonusFactor = 0;
		int64_t ClickTemporaryBonusDuration = 0;
		int64_t ClickTemporaryBonusTimer = 0;

		size_t NumberOfUpgrades = 0;
		Upgrade* Upgrades = nullptr;

		int64_t WakeTimer = 0;
		int64_t WakeDuration = 0;

		Data()
		{
			this->Score = 0;
			this->PassiveSpeed = 0;
			this->FrameCount = 0;
			this->TargetScore = 0;
			this->ClickValue = 0;
			this->GlobalFactor = 0;
			this->ClickTemporaryBonusFactor = 0;
			this->ClickTemporaryBonusDuration = 0;
			this->ClickTemporaryBonusTimer = 0;
			this->NumberOfUpgrades = 0;
			this->WakeTimer = 0;
			this->WakeDuration = 0;
			this->Upgrades = nullptr;
		};

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
			c.ClickTemporaryBonusFactor = this->ClickTemporaryBonusFactor;
			c.ClickTemporaryBonusDuration = this->ClickTemporaryBonusDuration;
			c.ClickTemporaryBonusTimer = this->ClickTemporaryBonusTimer;
			c.WakeDuration = this->WakeDuration;
			c.WakeTimer = this->WakeTimer;

			if (this->NumberOfUpgrades != c.NumberOfUpgrades)
			{
				if (c.Upgrades != nullptr)
				{
					delete[] c.Upgrades;
				}
				
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
		
		static FailureFlags GetUpgradeFailureFlags(Data* data, int index);

		friend std::ostream& operator<<(std::ostream&, const AutoClicker* c);

	private:
		std::vector<UpgradeDefinition> upgradeDefinitions;
		Data* data = nullptr;
		Data initialData;

		void inline ComputeUpgradeEffects();
	};
}