#include "pch.h"
#include "AutoClicker.h"

namespace AutoClicker
{
	Data::Data()
	{
		this->NumberOfUpgrades = 0;
		this->Upgrades = nullptr;
	}

	Data::Data(const std::vector<UpgradeDefinition>& definitions)
	{
		this->NumberOfUpgrades = definitions.size();
		if (this->NumberOfUpgrades > 0)
		{
			this->Upgrades = new Upgrade[this->NumberOfUpgrades];
			for (int index = 0; index < this->NumberOfUpgrades; ++index)
			{
				this->Upgrades[index].Definition = &definitions[index];
				this->Upgrades[index].InstanceBought = 0;
			}
		}
	}

	Data::~Data()
	{
		if (this->Upgrades != nullptr)
		{
			delete[] this->Upgrades;
			this->Upgrades = nullptr;
		}
	}

	AutoClicker::AutoClicker()
	{
	}

	AutoClicker::~AutoClicker()
	{
		if (this->data != nullptr)
		{
			delete this->data;
			this->data = nullptr;
		}
	}

	void AutoClicker::Initialize(std::vector<UpgradeDefinition>& uDefinitions)
	{
		this->upgradeDefinitions = uDefinitions;
		this->data = new Data(upgradeDefinitions);
		
		this->data->TickCount = 0;
		this->data->Score = 0;

		this->data->TargetScore = 1000000;
		this->data->PassiveSpeed = 0;
		this->data->ClickValue = 1;
	}

	void AutoClicker::Tick()
	{
		long stock = this->data->PassiveSpeed;
		for (int index = 0; index < this->data->NumberOfUpgrades; ++index)
		{
			stock += this->data->Upgrades[index].InstanceBought * this->data->Upgrades[index].Definition->ImpactValue;
		}

		this->data->Score += stock;
		++this->data->TickCount;
	}

	void AutoClicker::Click()
	{
		this->data->Score += this->data->ClickValue;
	}

	bool AutoClicker::BuyUpgrade(int index)
	{
		if (index < 0 || index >= this->data->NumberOfUpgrades)
		{
			return false;
		}
		
		long price = this->data->Upgrades[index].Price();

		if (price > this->data->Score)
		{
			return false;
		}

		if (this->data->Upgrades[index].Definition->Unique && this->data->Upgrades[index].InstanceBought > 0)
		{
			return false;
		}

		++this->data->Upgrades[index].InstanceBought;
		return true;
	}

	bool AutoClicker::IsOver()
	{
		return this->data->Score >= this->data->TargetScore;
	}

	std::ostream& operator<<(std::ostream& stream, const AutoClicker* c)
	{
		stream << "Score : " << c->data->Score << " Speed : " << c->data->PassiveSpeed << " Target : " << c->data->TargetScore;
		return stream;
	}
}
