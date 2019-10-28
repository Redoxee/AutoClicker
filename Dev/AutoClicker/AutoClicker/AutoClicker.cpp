// AutoClicker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Clicker.h"


int main()
{
 
	std::cout << "Starting Auto Clicker.\n";
	bool endRequested = false;
	Clicker* clicker = new Clicker();

	while (!endRequested)
	{
		clicker->Tick();

		if (clicker->IsOver())
		{
			endRequested |= true;
		}
	}

	std::cout << clicker << "\n";

	free(clicker);
	
	std::cout << "Auto Clicker end.";
}
