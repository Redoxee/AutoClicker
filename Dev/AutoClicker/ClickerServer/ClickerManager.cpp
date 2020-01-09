#include "ClickerManager.h"
#include <chrono>
#include "vector"
#include <stdio.h>

ClickerManager::ClickerManager()
{
}

AutoClicker::ValueIncreaseStrategy ParseIncreaseStrategy(const json::value jsonStrategy)
{
	AutoClicker::ValueIncreaseStrategy strategy;

	string_t type = jsonStrategy.at(U("Type")).as_string();
	if (type == U("Flat"))
	{
		strategy.Type = AutoClicker::ValueIncreaseType::Flat;
	}
	else if (type == U("Factor"))
	{
		strategy.Type = AutoClicker::ValueIncreaseType::Factor;
	}
	else if (type == U("Exponential"))
	{
		strategy.Type = AutoClicker::ValueIncreaseType::Exponential;
	}

	strategy.Rate = jsonStrategy.at(U("Value")).as_number().to_int64();

	return strategy;
}

void ClickerManager::Initialize(const json::value& configuration)
{
	if (configuration.has_field(U("Upgrades")))
	{
		json::array upgradeArray = configuration.at(U("Upgrades")).as_array();
		size_t numberOfUpgrades = upgradeArray.size();
		for (size_t index = 0; index < numberOfUpgrades; ++index)
		{
			json::value jsonUpgrade = upgradeArray.at(index);
			AutoClicker::UpgradeDefinition upgrade;
			upgrade.Name = jsonUpgrade[U("Name")].as_string();
			upgrade.Description = jsonUpgrade[U("Description")].as_string();

			upgrade.BasePrice = jsonUpgrade[U("BasePrice")].as_number().to_int64();
			upgrade.Unique = jsonUpgrade[U("Unique")].as_bool();

			string_t type = jsonUpgrade[U("UpgradeType")].as_string();
			if (type == U("ClickValue"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::ClickValue;
			}
			else if (type == U("Generator"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::Generator;
			}
			else if (type == U("UpgradeImprove"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::UpgradeImprove;
			}
			else if (type == U("Prestige"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::Prestige;
			}

			if (jsonUpgrade.has_field(U("BaseImpactValue")))
			{
				upgrade.BaseImpactValue = jsonUpgrade[U("BaseImpactValue")].as_number().to_int64();
			}

			json::value jsonPriceIncrease = jsonUpgrade[U("PriceStrategy")];
			upgrade.PriceIncreaseStrategy = ParseIncreaseStrategy(jsonPriceIncrease);
			if (jsonUpgrade.has_object_field(U("ImpactStrategy")))
			{
				json::value jsonImpact = jsonUpgrade[U("ImpactStrategy")];
				upgrade.Impact = ParseIncreaseStrategy(jsonImpact);
			}

			this->upgradeDefinitions.push_back(upgrade);
		}
	}

	this->clickerInstance.Initialize(this->upgradeDefinitions);

	this->lastUpdate = std::chrono::steady_clock::now();

	if (configuration.has_field(U("InitialState")))
	{
		string_t initialState = configuration.at(U("InitialState")).as_string();
		if (initialState == U("Paused"))
		{
			this->paused = true;
		}
	}
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
			upgrade[U("ImpactValue")] = data.Upgrades[index].CurrentImpactValue;

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
