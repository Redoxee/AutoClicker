#pragma once

#include <iostream>
#include <vector>

#include "Upgrades.h"
#include "Generator.h"

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

		size_t NumberOfGenerators;
		Generator* Generators;

		Data();
		Data(const std::vector<GeneratorDefinition>& generatorDefinitions);
		~Data();

		void CopyTo(Data& c) const
		{
			c.Score = this->Score;
			c.PassiveSpeed = this->PassiveSpeed;
			c.TickCount = this->TickCount;
			c.TargetScore = this->TargetScore;
			c.ClickValue = this->ClickValue;

			if (this->NumberOfGenerators != c.NumberOfGenerators)
			{
				delete[] c.Generators;
				c.NumberOfGenerators = this->NumberOfGenerators;
				c.Generators = new Generator[c.NumberOfGenerators];
			}
			
			for (int index = 0; index < this->NumberOfGenerators; ++index)
			{
				c.Generators[index] = this->Generators[index];
			}
		}
	};

	class AutoClicker
	{
	public:
		AutoClicker();
		~AutoClicker();

		void Initialize(std::vector<GeneratorDefinition>& generatorDefinition);

		void Tick();
		void Click();
		bool IsOver();
		bool BuyGenerator(int index);

		void GetData(Data& d) const
		{
			this->data->CopyTo(d);
		}

		friend std::ostream& operator<<(std::ostream&, const AutoClicker* c);

	private:
		std::vector<GeneratorDefinition> generatorDefinitions;

		Data* data = nullptr;
	};
}