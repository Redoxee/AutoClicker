#pragma once
#include "AutoClicker.h"
#include <queue>
#include "cpprest/json.h"
#include <thread>
#include <mutex>

enum Order
{
	Tick,
	Click,
	Terminate,
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

private:
	bool isTerminated = false;

	AutoClicker::AutoClicker clickerInstance;
	std::queue<Order> pendingOrder;
	AutoClicker::Data buffer;
	
	std::thread clickerThread;
	std::mutex mutex;

	void ManagerThreadLoop();
	void Synchonize();
};
