#pragma once
#include "AutoClicker.h"
#include <queue>

enum Order
{
	Tick,
	Terminate,
};

class ClickerManager
{
public:
	void PostOrder(Order order);
	void ProcessNextOrder();
	bool IsTerminated();

private:
	AutoClicker::AutoClicker clickerInstance;
	std::queue<Order> pendingOrder;

	bool isTerminated = false;
};
