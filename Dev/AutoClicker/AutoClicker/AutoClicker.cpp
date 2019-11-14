#include "pch.h"
#include "AutoClicker.h"

namespace AutoClicker
{
	Data::Data()
	{
	}

	Data::Data(int numberOfDefinition)
	{
		this->UpgradeBought = new long[numberOfDefinition];
	}

	Data::~Data()
	{
		if (this->UpgradeBought != nullptr)
		{
			delete(this->UpgradeBought);
		}
	}

	AutoClicker::AutoClicker()
	{
	}

	AutoClicker::~AutoClicker()
	{
	}

	void AutoClicker::Initialize(UpgradeDefinition* upgradesDefinition, int numberOfUpgrades)
	{
		this->UpgradeDefinitions = upgradesDefinition;
		this->numberOfUpgrades = numberOfUpgrades;

		this->data = Data(numberOfUpgrades);

		this->data.TargetScore = 1000000;
		this->data.PassiveSpeed = 1;
		this->data.ClickValue = 1;
	
		for (int index = 0; index < numberOfUpgrades; ++index)
		{
			this->data.UpgradeBought[index] = 0;
		}
	}

	void AutoClicker::Tick()
	{
		this->data.Score += this->data.PassiveSpeed;
		++this->data.TickCount;
	}

	void AutoClicker::Click()
	{
		this->data.Score += this->data.ClickValue;
	}

	bool AutoClicker::IsOver()
	{
		return this->data.Score >= this->data.TargetScore;
	}

	std::ostream& operator<<(std::ostream& stream, const AutoClicker* c)
	{
		stream << "Score : " << c->data.Score << " Speed : " << c->data.PassiveSpeed << " Target : " << c->data.TargetScore;
		return stream;
	}
}
