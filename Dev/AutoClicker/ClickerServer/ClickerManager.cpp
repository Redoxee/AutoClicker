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
	else if (type == U("Overwrite"))
	{
		strategy.Type = AutoClicker::ValueIncreaseType::Overwrite;
	}

	strategy.Rate = jsonStrategy.at(U("Value")).as_number().to_double();

	return strategy;
}

void ClickerManager::Initialize(const json::value& jsonConfiguration)
{
	wcout << "Parsing clicker configuration." << endl;
	AutoClicker::ConfigurationData configData;

	configData.Score = 0;
	if (jsonConfiguration.has_number_field(U("InitialScore")))
	{
		configData.Score = jsonConfiguration.at(U("InitialScore")).as_number().to_int64();
	}

	configData.ClickValue = 1;
	if (jsonConfiguration.has_number_field(U("InitialClickValue")))
	{
		configData.ClickValue = jsonConfiguration.at(U("InitialClickValue")).as_number().to_int64();
	}

	configData.TargetScore = 1000000;
	if (jsonConfiguration.has_number_field(U("TargetScore")))
	{
		configData.TargetScore = jsonConfiguration.at(U("TargetScore")).as_number().to_int64();
	}

	configData.PassiveSpeed = 0;
	if (jsonConfiguration.has_number_field(U("InitialPassiveSpeed")))
	{
		configData.PassiveSpeed = jsonConfiguration.at(U("InitialPassiveSpeed")).as_number().to_int64();
	}

	configData.GlobalFactor = 1;
	if (jsonConfiguration.has_number_field(U("InitialGlobalFactor")))
	{
		configData.GlobalFactor = jsonConfiguration.at(U("InitialGlobalFactor")).as_number().to_int64();
	}

	if (jsonConfiguration.has_number_field(U("InitialTemporaryBoostDuration")))
	{
		configData.TempBoostDuration =  jsonConfiguration.at(U("InitialTemporaryBoostDuration")).as_number().to_uint64();
	}

	if (jsonConfiguration.has_number_field(U("InitialWakeDuration")))
	{
		configData.WakeDuration = jsonConfiguration.at(U("InitialWakeDuration")).as_number().to_uint64();
	}

	if (jsonConfiguration.has_field(U("Upgrades")))
	{
		json::array upgradeArray = jsonConfiguration.at(U("Upgrades")).as_array();
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
			else if (type == U("UpgradeImprovement"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::UpgradeImprove;
			}
			else if (type == U("Prestige"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::Prestige;
			}
			else if (type == U("ClickTemporaryBoostDuration"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::ClickTemporaryBoostDuration;
			}
			else if (type == U("ClickTemporaryBoostStrength"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::ClickTemporaryBoostFactor;
			}
			else if (type == U("WakeDuration"))
			{
				upgrade.UpgradeType = AutoClicker::UpgradeType::WakeDuration;
			}
			else
			{
				wcout << "Unknown improvement type : " << type << "." << endl;
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

		for (size_t index = 0; index < numberOfUpgrades; ++index)
		{
			json::value jsonUpgrade = upgradeArray.at(index);
			
			if (jsonUpgrade.has_field(U("TargetName")))
			{
				string_t targetName = jsonUpgrade.at(U("TargetName")).as_string();

				int targetIndex = -1;

				for (int jndex = 0; jndex < numberOfUpgrades; ++jndex)
				{
					if (this->upgradeDefinitions[jndex].Name == targetName)
					{
						targetIndex = jndex;
						break;
					}
				}

				if (targetIndex > -1)
				{
					this->upgradeDefinitions[index].TargetInfo = targetIndex;
				}
			}

			if (jsonUpgrade.has_field(U("Lock")))
			{
				json::value jsonLock = jsonUpgrade.at(U("Lock"));
				this->upgradeDefinitions[index].Lock.targetValue = jsonLock.at(U("Value")).as_number().to_int64();
				string_t comparer = jsonLock.at(U("Comparer")).as_string();
				if (comparer == U("SmallerThan"))
				{
					this->upgradeDefinitions[index].Lock.Comparer = AutoClicker::Comparer::Smaller;
				}
				else
				{
					this->upgradeDefinitions[index].Lock.Comparer = AutoClicker::Comparer::Greater;
				}

				string_t targetName = jsonLock.at(U("TargetName")).as_string();

				int targetIndex = -1;

				for (int jndex = 0; jndex < numberOfUpgrades; ++jndex)
				{
					if (this->upgradeDefinitions[jndex].Name == targetName)
					{
						targetIndex = jndex;
						break;
					}
				}

				if (targetIndex > -1)
				{
					this->upgradeDefinitions[index].Lock.LockIndex = targetIndex;
				}
			}
		}

		configData.UpgradeDefinitions = this->upgradeDefinitions;
	}

	this->clickerInstance.Initialize(configData);

	this->lastUpdate = std::chrono::steady_clock::now();

	if (jsonConfiguration.has_field(U("InitialState")))
	{
		string_t initialState = jsonConfiguration.at(U("InitialState")).as_string();
		if (initialState == U("Paused"))
		{
			this->paused = true;
		}
	}

	wcout << "Clicker configuration parsed." << endl;
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
	case OrderIdentifier::DoFrame:
		if (this->clickerInstance.IsOver())
		{
			break;
		}

		this->clickerInstance.Update();
		
		break;

	case OrderIdentifier::Click:
		if (this->clickerInstance.IsOver())
		{
			break;
		}
		
		this->clickerInstance.Click();
		break;

	case OrderIdentifier:: BuyUpgrade:
	{
		if (this->clickerInstance.IsOver())
		{
			break;
		}

		int index = order.Value;
		if (index < 0 || index >= this->upgradeDefinitions.size())
		{
			wcerr << "When buyting generator index out of range " << order.Value << endl;
		}
		else
		{
			this->clickerInstance.BuyUpgrade(index);
		}

		break;
	}
	case OrderIdentifier::Meta_Terminate:
		this->isTerminated = true;
		break;

	case OrderIdentifier::Meta_FrameWait:
		this->SetFrameWait(order.Value);
		break;

	case OrderIdentifier::Meta_PauseUpdate:
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

				this->lastUpdate = chrono::steady_clock::now();
			}
		}
	}
	wcout << "Exiting the clicker manager thread loop" << endl;
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
	
	result[U("TemporaryBonusTimer")] = data.ClickTemporaryBonusTimer;
	result[U("TemporaryBonusFactor")] = data.ClickTemporaryBonusFactor;

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
			upgrade[U("FailureFlags")] = static_cast<int>(AutoClicker::AutoClicker::GetUpgradeFailureFlags(&data ,index));

			upgrades[index] = upgrade;
		}

		result[U("Upgrades")] = upgrades;
	}

	return result;
}
