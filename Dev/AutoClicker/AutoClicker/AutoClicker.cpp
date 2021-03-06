#include "pch.h"
#include "AutoClicker.h"

namespace AutoClicker
{
	Data::Data(const std::vector<UpgradeDefinition>& definitions) : Data()
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
				this->Upgrades[index].CurrentImpactValue = definitions[index].BaseImpactValue;
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

	void AutoClicker::Initialize(const ConfigurationData& configuration)
	{
		this->upgradeDefinitions = configuration.UpgradeDefinitions;
		this->data = new Data(upgradeDefinitions);
		
		this->data->FrameCount = 0;
		this->data->Score = configuration.Score;
		this->data->GlobalFactor = configuration.GlobalFactor;
		this->data->TargetScore = configuration.TargetScore;

		this->data->PassiveSpeed = configuration.PassiveSpeed;
		this->data->ClickValue = configuration.ClickValue;

		this->data->ClickTemporaryBonusDuration = configuration.TempBoostDuration;
		this->data->WakeDuration = configuration.WakeDuration;
		this->data->WakeTimer = configuration.WakeDuration;

		this->data->CopyTo(this->initialData);
	}

	void AutoClicker::Update()
	{
		if (this->data->WakeDuration > 0)
		{
			if (this->data->WakeTimer < 1)
			{
				return;
			}

			this->data->WakeTimer--;
		}

		int64_t stock = this->data->PassiveSpeed;
		stock *= this->data->GlobalFactor;

		if (this->data->ClickTemporaryBonusFactor > 1 && this->data->ClickTemporaryBonusTimer > 0)
		{
			stock *= this->data->ClickTemporaryBonusFactor;
			--this->data->ClickTemporaryBonusTimer;
		}

		this->data->Score += stock;

		++this->data->FrameCount;
	}

	void AutoClicker::Click()
	{
		if (this->data->ClickTemporaryBonusFactor > 1 && this->data->ClickTemporaryBonusDuration)
		{
			this->data->ClickTemporaryBonusTimer = this->data->ClickTemporaryBonusDuration;
		}

		int64_t stock = this->data->ClickValue * this->data->GlobalFactor;
		this->data->Score += stock;

		this->data->WakeTimer = this->data->WakeDuration;
		
		++this->data->FrameCount;
	}

	bool AutoClicker::BuyUpgrade(int upgradeIndex)
	{		
		FailureFlags failures =  AutoClicker::GetUpgradeFailureFlags(this->data, upgradeIndex);
		if (failures != FailureFlags::None)
		{
			return false;
		}

		int64_t price = this->data->Upgrades[upgradeIndex].Price;

		if (price > this->data->Score)
		{
			return false;
		}

		Upgrade* upgrade = &this->data->Upgrades[upgradeIndex];
		const UpgradeDefinition* upgradeDefinition = upgrade->Definition;
		UpgradeType upgradeType = upgradeDefinition->UpgradeType;
		
		if (upgradeType == UpgradeType::Prestige)
		{
			std::size_t numberOfUpgrades = this->data->NumberOfUpgrades;
			for (int index = 0; index < numberOfUpgrades; ++index)
			{
				Upgrade* targetUpgrade = &this->data->Upgrades[index];
				if (targetUpgrade->Definition->UpgradeType == UpgradeType::Prestige)
				{
					continue;
				}
				
				targetUpgrade->InstanceBought = 0;
				targetUpgrade->Price = targetUpgrade->Definition->BasePrice;
				targetUpgrade->CurrentImpactValue = targetUpgrade->Definition->BaseImpactValue;
			}

			this->data->GlobalFactor += upgrade->CurrentImpactValue;
			upgrade->CurrentImpactValue = upgrade->Definition->BaseImpactValue;
			this->data->Score = 0;
			price = 0;
		}
		else if (upgradeType == UpgradeType::UpgradeImprove)
		{
			Upgrade* targetUpgrade = &this->data->Upgrades[upgradeDefinition->TargetInfo];
			upgradeDefinition->Impact.ComputeNextValue(targetUpgrade->CurrentImpactValue, upgradeDefinition->BaseImpactValue, this->data->Upgrades[upgradeIndex].InstanceBought + 1);
		}

		this->data->Score -= price;
		++this->data->Upgrades[upgradeIndex].InstanceBought;

		this->ComputeUpgradeEffects();

		// TODO if doubles are used to compute the next price then there is no point in using long to store the data.
		// I don't know how to do floating pow function to circumvent this limitation.
		// I Don't expect the prices to go as high though.
		this->data->Upgrades[upgradeIndex].ComputePrice();

		this->data->WakeTimer = this->data->WakeDuration;

		++this->data->FrameCount;
		return true;
	}

	void inline AutoClicker::ComputeUpgradeEffects()
	{
		this->data->ClickValue = 1;
		this->data->PassiveSpeed = 0;

		this->data->ClickTemporaryBonusFactor = 1;
		this->data->ClickTemporaryBonusDuration = this->initialData.ClickTemporaryBonusDuration;
		this->data->WakeDuration = this->initialData.WakeDuration;

		std::size_t numberOfUpgrades = this->data->NumberOfUpgrades;
		for (int upgradeIndex = 0; upgradeIndex < numberOfUpgrades; ++upgradeIndex)
		{
			Upgrade* upgrade = &this->data->Upgrades[upgradeIndex];
			const UpgradeDefinition* upgradeDefinition = upgrade->Definition;
			UpgradeType upgradeType = upgradeDefinition->UpgradeType;
			if (upgradeType == UpgradeType::ClickValue)
			{
				this->data->ClickValue += upgrade->CurrentImpactValue * upgrade->InstanceBought;
			}
			else if (upgradeType == UpgradeType::Generator)
			{
				this->data->PassiveSpeed += upgrade->CurrentImpactValue * upgrade->InstanceBought;
			}
			else if (upgradeType == UpgradeType::ClickTemporaryBoostDuration)
			{
				this->data->ClickTemporaryBonusDuration += upgrade->CurrentImpactValue * upgrade->InstanceBought;
			}
			else if (upgradeType == UpgradeType::ClickTemporaryBoostFactor)
			{
				this->data->ClickTemporaryBonusFactor += upgrade->CurrentImpactValue * upgrade->InstanceBought;
			}
			else if (upgradeType == UpgradeType::WakeDuration)
			{
				this->data->WakeDuration += upgrade->CurrentImpactValue * upgrade->InstanceBought;
			}
		}
	}

	bool AutoClicker::IsOver()
	{
		return this->data->Score >= this->data->TargetScore;
	}

	bool Compare(int64_t a, Comparer sigil, int64_t b)
	{
		switch (sigil)
		{
		case Comparer::Smaller:
			return a <= b;
		case Comparer::Greater:
			return a >= b;
		default:
			return false;
		}
	}

	FailureFlags AutoClicker::GetUpgradeFailureFlags(Data* data, int upgradeIndex)
	{
		if (upgradeIndex < 0 || upgradeIndex >= data->NumberOfUpgrades)
		{
			return FailureFlags::UnknownUpgrade;
		}

		FailureFlags failures = FailureFlags::None;

		int64_t price = data->Upgrades[upgradeIndex].Price;

		if (price > data->Score)
		{
			failures = failures | FailureFlags::NotEnoughMoney;
		}

		if (data->Upgrades[upgradeIndex].Definition->Unique && data->Upgrades[upgradeIndex].InstanceBought > 0)
		{
			failures = failures | FailureFlags::PurchaseLimitReached;
		}

		if (data->Score >= data->TargetScore)
		{
			failures = failures | FailureFlags::GameOver;
		}

		if (data->Upgrades[upgradeIndex].Definition->Lock.LockIndex > -1)
		{
			int64_t currentValue = data->Upgrades[data->Upgrades[upgradeIndex].Definition->Lock.LockIndex].InstanceBought;
			int64_t targetValue = data->Upgrades[upgradeIndex].Definition->Lock.targetValue;
			if (!Compare(currentValue, data->Upgrades[upgradeIndex].Definition->Lock.Comparer, targetValue))
			{
				failures = failures | FailureFlags::LockedByAnOtherPurchase;
			}
		}

		return failures;
	}

	std::ostream& operator<<(std::ostream& stream, const AutoClicker* c)
	{
		stream << "Score : " << c->data->Score << " Speed : " << c->data->PassiveSpeed << " Target : " << c->data->TargetScore;
		return stream;
	}
}
