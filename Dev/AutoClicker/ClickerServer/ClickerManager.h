#pragma once
#include "AutoClicker.h"
#include <queue>
#include "cpprest/json.h"
#include <thread>
#include <mutex>
#include "vector"

enum OrderIdentifier
{
	Tick,
	Click,
	BuyGenerator,
	Meta_Terminate,
	Meta_TickLength,
};

class Order
{
public:
	Order(OrderIdentifier id, int value)
	{
		this->Identifier = id;
		this->Value = value;
	}

	OrderIdentifier Identifier = OrderIdentifier::Tick;
	int Value = 0;
};

class ClickerManager
{
public:
	ClickerManager();
	~ClickerManager();

	void PostOrder(Order order);
	void ProcessNextOrder();
	bool IsTerminated();

	void StartClickerThread();
	web::json::value GetDataAsJson();

	void SetTickLength(long length)
	{
		this->tickLength = length;
	}

private:
	std::vector<AutoClicker::GeneratorDefinition> generatorDefinitions;

	bool isTerminated = false;

	AutoClicker::AutoClicker clickerInstance;
	std::queue<Order> pendingOrder;
	AutoClicker::Data buffer;
	
	std::thread clickerThread;
	std::mutex mutex;

	void ManagerThreadLoop();
	void Synchonize();
	long tickLength = 100;
};
