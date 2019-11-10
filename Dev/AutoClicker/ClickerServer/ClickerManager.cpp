#include "ClickerManager.h"

void ClickerManager::PostOrder(Order order)
{
	this->pendingOrder.push(order);
}

bool ClickerManager::IsTerminated()
{
	return this->isTerminated;
}

void ClickerManager::ProcessNextOrder()
{
	if (this->pendingOrder.size == 0)
	{
		return;
	}

	Order order = this->pendingOrder.front;
	this->pendingOrder.pop();

	switch (order)
	{
	case Tick:
		this->clickerInstance.Tick();
		break;
	case Terminate:
		this->isTerminated = true;
		break;
	default:
		break;
	}
}