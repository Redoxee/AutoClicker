#include "pch.h"
#include "AutoClicker.h"

namespace AutoClicker
{
	Data::Data()
	{
		this->NumberOfGenerators = 0;
		this->Generators = nullptr;
	}

	Data::Data(const std::vector<GeneratorDefinition>& definitions)
	{
		this->NumberOfGenerators = definitions.size();
		if (this->NumberOfGenerators > 0)
		{
			this->Generators = new Generator[this->NumberOfGenerators];
			for (int index = 0; index < this->NumberOfGenerators; ++index)
			{
				this->Generators[index].Definition = &definitions[index];
				this->Generators[index].InstanceBought = 0;
			}
		}
	}

	Data::~Data()
	{
		if (this->Generators != nullptr)
		{
			delete[] this->Generators;
			this->Generators = nullptr;
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

	void AutoClicker::Initialize(std::vector<GeneratorDefinition>& gDefinitions)
	{
		this->generatorDefinitions = gDefinitions;
		this->data = new Data(generatorDefinitions);
		
		this->data->TickCount = 0;
		this->data->Score = 0;

		this->data->TargetScore = 1000000;
		this->data->PassiveSpeed = 0;
		this->data->ClickValue = 1;
	}

	void AutoClicker::Tick()
	{
		long stock = this->data->PassiveSpeed;
		for (int index = 0; index < this->data->NumberOfGenerators; ++index)
		{
			stock += this->data->Generators[index].InstanceBought * this->data->Generators[index].Definition->BaseRate;
		}

		this->data->Score += stock;
		++this->data->TickCount;
	}

	void AutoClicker::Click()
	{
		this->data->Score += this->data->ClickValue;
	}

	bool AutoClicker::BuyGenerator(int index)
	{
		if (index < 0 || index >= this->data->NumberOfGenerators)
		{
			return false;
		}
		
		long price = this->data->Generators[index].Price();

		if (price > this->data->Score)
		{
			return false;
		}

		++this->data->Generators[index].InstanceBought;
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
