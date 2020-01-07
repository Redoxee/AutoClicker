#pragma once
#include "AutoClicker.h"
#include <queue>
#include "cpprest/json.h"
#include <thread>
#include <mutex>
#include <chrono>
#include "vector"

using namespace web;

enum OrderIdentifier
{
	DoFrame,
	Click,
	BuyUpgrade,
	Meta_Terminate,
	Meta_FrameWait,
	Meta_PauseUpdate,
};

class Order
{
public:
	Order(OrderIdentifier id, int value)
	{
		this->Identifier = id;
		this->Value = value;
	}

	OrderIdentifier Identifier = OrderIdentifier::DoFrame;
	int Value = 0;
};

class ClickerManager
{
public:
	ClickerManager();
	~ClickerManager();

	void Initialize(const json::value& configuration);

	void PostOrder(Order order);
	void ProcessNextOrder();
	bool IsTerminated();

	void StartClickerThread();
	web::json::value GetDataAsJson();

	void SetFrameWait(long length)
	{
		this->frameWait = length;
	}

	bool IsPaused()
	{
		return this->paused;
	}

	void SetPause(bool pause)
	{
		this->paused = pause;
	}

private:
	std::vector<AutoClicker::UpgradeDefinition> upgradeDefinitions;

	bool isTerminated = false;

	AutoClicker::AutoClicker clickerInstance;
	std::queue<Order> pendingOrder;
	AutoClicker::Data buffer;
	
	std::thread clickerThread;
	std::mutex mutex;
	std::chrono::steady_clock::time_point lastUpdate;

	void ManagerThreadLoop();
	void Synchonize();
	long frameWait = 100;

	bool paused;
};
