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
				this->Upgrades[index].Price = definitions[index].BasePrice;
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
		
		this->data->FrameCount = 0;
		this->data->Score = 0;
		this->data->GlobalFactor = 1;
		this->data->TargetScore = 1000000;

		this->data->PassiveSpeed = 0;
		this->data->ClickValue = 1;
	}

	void AutoClicker::Update()
	{
		long stock = this->data->PassiveSpeed;
		stock *= this->data->GlobalFactor;
		this->data->Score += stock;
		++this->data->FrameCount;
	}

	void AutoClicker::Click()
	{
		long stock = this->data->ClickValue * this->data->GlobalFactor;
		this->data->Score += stock;
	}

	bool AutoClicker::BuyUpgrade(int upgradeIndex)
	{
		if (upgradeIndex < 0 || upgradeIndex >= this->data->NumberOfUpgrades)
		{
			return false;
		}
		
		long price = this->data->Upgrades[upgradeIndex].Price;

		if (price > this->data->Score)
		{
			return false;
		}

		if (this->data->Upgrades[upgradeIndex].Definition->Unique && this->data->Upgrades[upgradeIndex].InstanceBought > 0)
		{
			return false;
		}

		const UpgradeDefinition* upgradeDefinition = this->data->Upgrades[upgradeIndex].Definition;
		UpgradeType upgradeType = upgradeDefinition->UpgradeType;
		if (upgradeType == UpgradeType::ClickValue)
		{
			this->data->ClickValue += upgradeDefinition->ImpactValue;
		}
		else if (upgradeType == UpgradeType::Generator)
		{
			this->data->PassiveSpeed += upgradeDefinition->ImpactValue;
		}
		else if (upgradeType == UpgradeType::Prestige)
		{
			int numberOfUpgrades = this->data->NumberOfUpgrades;
			for (int index = 0; index < numberOfUpgrades; ++index)
			{
				if (upgradeIndex == index)
				{
					continue;
				}

				this->data->Upgrades[index].InstanceBought = 0;
				this->data->Upgrades[index].Price = this->data->Upgrades[index].ComputeNextPrice();
			}

			this->data->PassiveSpeed = 0;
			this->data->ClickValue = 1;
			this->data->GlobalFactor += upgradeDefinition->ImpactValue;
		}

		this->data->Score -= price;
		++this->data->Upgrades[upgradeIndex].InstanceBought;

		// TODO if doubles are used to compute the next price then there is no point in using long to store the data.
		// I don't know how to do floating pow function to circumvent this limitation.
		// I Don't expect the prices to go as high though.
		this->data->Upgrades[upgradeIndex].Price = static_cast<long>(floor(this->data->Upgrades[upgradeIndex].ComputeNextPrice()));
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
