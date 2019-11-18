#include "ClickerManager.h"
#include <chrono>

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

	switch (order)
	{
	case Tick:
		this->clickerInstance.Tick();
		break;
	case Click:
		this->clickerInstance.Click();
		this->clickerInstance.Tick();
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
		if (!this->clickerInstance.IsOver())
		{
			this->clickerInstance.Tick();
			this->Synchonize();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
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

	size_t numberOfUpgrades = data.NumberOfUpgrades;
	for (int index = 0; index < numberOfUpgrades; ++index)
	{
		result[U("PurchasedUpgrades")][index] = data.UpgradeBought[index];
	}

	return result;
}
