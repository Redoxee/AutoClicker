#include "ClickerManager.h"
#include <chrono>
#include "vector"
#include <stdio.h>

ClickerManager::ClickerManager()
{
	AutoClicker::UpgradeDefinition generator;
	generator.UpgradeType = AutoClicker::UpgradeType::Generator;
	generator.BasePrice = 400;
	generator.ImpactValue = 1;
	generator.Name = "Small clicker";
	generator.Description = "Add two coin every frame";
	generator.Unique = false;
	generator.PriceIncreaseStrategy = AutoClicker::ValueIncreaseStrategy(AutoClicker::ValueIncreaseType::Exponential, 1.04);
	this->upgradeDefinitions.push_back(generator);

	AutoClicker::UpgradeDefinition clickerUpgrade;
	clickerUpgrade.UpgradeType = AutoClicker::UpgradeType::ClickValue;
	clickerUpgrade.ImpactValue = 1;
	clickerUpgrade.BasePrice = 50;
	clickerUpgrade.Unique = false;
	clickerUpgrade.Name = "Click upgrade";
	clickerUpgrade.Description = "Improve each click by one";
	clickerUpgrade.PriceIncreaseStrategy = AutoClicker::ValueIncreaseStrategy(AutoClicker::ValueIncreaseType::Factor, 4);
	this->upgradeDefinitions.push_back(clickerUpgrade);

	AutoClicker::UpgradeDefinition prestigeUpgrade;
	prestigeUpgrade.UpgradeType = AutoClicker::UpgradeType::Prestige;
	prestigeUpgrade.ImpactValue = 1;
	prestigeUpgrade.BasePrice = 1000;
	prestigeUpgrade.Name = "Prestige";
	prestigeUpgrade.Description = "Remove all upgrades but increase all income by two";
	prestigeUpgrade.Unique = true;
	this->upgradeDefinitions.push_back(prestigeUpgrade);

	this->clickerInstance.Initialize(this->upgradeDefinitions);

	this->lastUpdate = std::chrono::steady_clock::now();

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
	case DoFrame:
		this->clickerInstance.Update();
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

	case Meta_FrameWait:
		this->SetFrameWait(order.Value);
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
			std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(now - this->lastUpdate).count();

			if (elapsedTime >= this->frameWait)
			{
				if (!this->clickerInstance.IsOver())
				{
					this->clickerInstance.Update();
					this->Synchonize();
				}

				this->lastUpdate = std::chrono::steady_clock::now();
			}
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
	result[U("FrameCount")] = data.FrameCount;
	result[U("TargetScore")] = data.TargetScore;
	result[U("ClickValue")] = data.ClickValue;
	result[U("GlobalFactor")] = data.GlobalFactor;
	
	if (data.NumberOfUpgrades > 0)
	{
		auto upgrades = web::json::value::array();
		for (int index = 0; index < data.NumberOfUpgrades; ++index)
		{
			auto upgrade = web::json::value::object();
			upgrade[U("Price")] = data.Upgrades[index].Price;
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
