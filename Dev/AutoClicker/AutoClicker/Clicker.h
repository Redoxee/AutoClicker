#pragma once
#include <iostream>

class Clicker
{
public:
	Clicker();
	~Clicker();
	
	void Tick();
	bool IsOver();

	friend std::ostream& operator<<(std::ostream&, const Clicker* c);
private:
	long clickerScore = 0;
	long speed = 1;
	long TickCount = 0;
	long targetScore = 100;

	long clickSpeed = 1;
	long clockMultiplier = 1;
};