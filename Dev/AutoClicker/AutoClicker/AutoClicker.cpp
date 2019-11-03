#include "pch.h"
#include "AutoClicker.h"

namespace AutoClicker
{
	AutoClicker::AutoClicker()
	{
	}


	AutoClicker::~AutoClicker()
	{
	}

	void AutoClicker::Tick()
	{
		this->clickerScore += this->speed;
	}

	bool AutoClicker::IsOver()
	{
		return this->clickerScore >= this->targetScore;
	}

	std::ostream& operator<<(std::ostream& stream, const AutoClicker* c)
	{
		stream << "Score : " << c->clickerScore << " Speed : " << c->speed << " Target : " << c->targetScore;
		return stream;
	}
}
