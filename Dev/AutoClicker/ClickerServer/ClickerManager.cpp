#include "ClickerManager.h"
#include <chrono>
#include "vector"

ClickerManager::ClickerManager()
{
	AutoClicker::GeneratorDefinition generator;
	
	generator.BasePrice = 10;
	generator.BaseRate = 2;
	generator.PriceRate = 2;

	this->generatorDefinitions.push_back(generator);

	this->clickerInstance.Initialize(this->generatorDefinitions);
}

ClickerManager::~ClickerManager()
{
	this->isTerminated = true;
	this->clickerThread.join();
}

void ClickerManager::PostOrder(Order order)
{
	this->mutex.lock();
	this->pendingOrder.push(order);
	this->mutex.unlock();
}

bool ClickerManager::IsTerminated()
{
	return this->isTerminated;
}

void ClickerManager::ProcessNextOrder()
{
	if (this->pendingOrder.size() == 0)
	{
		return;
	}

	this->mutex.lock();
	Order& order = this->pendingOrder.front();
	this->pendingOrder.pop();
	this->mutex.unlock();

	switch (order.Identifier)
	{
	case Tick:
		this->clickerInstance.Tick();
		break;

	case Click:
		this->clickerInstance.Click();
		break;

	case BuyGenerator:
	{
		int index = order.Value;
		if (index < 0 || index >= this->generatorDefinitions.size())
		{
			cerr << "when buyting generator index out of range " << order.Value << endl;
		}
		else
		{
			this->clickerInstance.BuyGenerator(index);
		}

		break;
	}
	case Terminate:
		this->isTerminated = true;
		break;

	default:
		break;
	}

	this->Synchonize();
}

void ClickerManager::StartClickerThread()
{
	this->clickerThread = std::thread(&ClickerManager::ManagerThreadLoop, this);
}

void ClickerManager::ManagerThreadLoop()
{
	while (!this->IsTerminated())
	{
		this->ProcessNextOrder();

		if (!this->clickerInstance.IsOver())
		{
			this->clickerInstance.Tick();
			this->Synchonize();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	std::cout << "Exiting the clicker manager thread loop" << std::endl;
}

void ClickerManager::Synchonize()
{
	this->mutex.lock();
	this->clickerInstance.GetData(this->buffer);
	this->mutex.unlock();
}

web::json::value ClickerManager::GetDataAsJson()
{

	AutoClicker::Data data;
	this->mutex.lock();
	this->buffer.CopyTo(data);
	this->mutex.unlock();

	web::json::value result;
	result[U("Score")] = data.Score;
	result[U("PassiveSpeed")] = data.PassiveSpeed;
	result[U("TickCount")] = data.TickCount;
	result[U("TargetScore")] = data.TargetScore;
	result[U("ClickValue")] = data.ClickValue;
	
	if (data.NumberOfGenerators > 0)
	{
		auto generators = web::json::value::array();
		for (int index = 0; index < data.NumberOfGenerators; ++index)
		{
			auto generator = web::json::value::object();
			generator[U("Price")] = data.Generators[index].Price();
			generator[U("NumberOfInstanceBought")] = data.Generators[index].InstanceBought;
			generator[U("BaseRate")] = data.Generators[index].Definition->BaseRate;
			generators[index] = generator;
		}

		result[U("Generators")] = generators;
	}

	return result;
}
