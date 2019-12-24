#include "ClickerManager.h"
#include <chrono>
#include "vector"
#include <stdio.h>

ClickerManager::ClickerManager()
{
	AutoClicker::UpgradeDefinition generator;
	generator.UpgradeType = AutoClicker::UpgradeType::Generator;
	generator.BasePrice = 100;
	generator.ImpactValue = 2;
	generator.PriceRate = 2;
	generator.Name = "Small clicker";
	generator.Description = "Add two coin every tick";
	generator.Unique = false;
	this->upgradeDefinitions.push_back(generator);

	AutoClicker::UpgradeDefinition clickerUpgrade;
	clickerUpgrade.UpgradeType = AutoClicker::UpgradeType::ClickValue;
	clickerUpgrade.ImpactValue = 1;
	clickerUpgrade.BasePrice = 10;
	clickerUpgrade.PriceRate = 2;
	clickerUpgrade.Unique = false;
	clickerUpgrade.Name = "Click upgrade";
	clickerUpgrade.Description = "Improve each click by one";
	this->upgradeDefinitions.push_back(clickerUpgrade);

	this->clickerInstance.Initialize(this->upgradeDefinitions);
	this->paused = true;
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

	case BuyUpgrade:
	{
		int index = order.Value;
		if (index < 0 || index >= this->upgradeDefinitions.size())
		{
			std::cerr << "when buyting generator index out of range " << order.Value << std::endl;
		}
		else
		{
			this->clickerInstance.BuyUpgrade(index);
		}

		break;
	}
	case Meta_Terminate:
		this->isTerminated = true;
		break;

	case Meta_TickLength:
		this->SetTickLength(order.Value);
		break;

	case Meta_PauseUpdate:
		this->SetPause(order.Value != 0);
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

		if (!this->paused)
		{
			if (!this->clickerInstance.IsOver())
			{
				this->clickerInstance.Tick();
				this->Synchonize();
			}
		}

		if (this->tickLength > 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(this->tickLength));
		}
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
	
	if (data.NumberOfUpgrades > 0)
	{
		auto upgrades = web::json::value::array();
		for (int index = 0; index < data.NumberOfUpgrades; ++index)
		{
			auto upgrade = web::json::value::object();
			upgrade[U("Price")] = data.Upgrades[index].Price();
			upgrade[U("NumberOfInstanceBought")] = data.Upgrades[index].InstanceBought;
			upgrade[U("BaseRate")] = data.Upgrades[index].Definition->ImpactValue;

			utility::string_t name = utility::conversions::to_string_t(data.Upgrades[index].Definition->Name);
			upgrade[U("Name")] = web::json::value::string(name);

			utility::string_t description = utility::conversions::to_string_t(data.Upgrades[index].Definition->Description);
			upgrade[U("Description")] = web::json::value::string(description);

			upgrades[index] = upgrade;
		}

		result[U("Upgrades")] = upgrades;
	}

	return result;
}
