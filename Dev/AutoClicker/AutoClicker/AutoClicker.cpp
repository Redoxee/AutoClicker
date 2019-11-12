#include "pch.h"
#include "AutoClicker.h"

namespace AutoClicker
{
	AutoClicker::AutoClicker()
	{
		this->data.TargetScore = 1000000;
		this->data.PassiveSpeed = 1;
		this->data.ClickValue = 1;
	}


	AutoClicker::~AutoClicker()
	{
	}

	void AutoClicker::Tick()
	{
		this->data.Score += this->data.PassiveSpeed;
		++this->data.TickCount;
	}

	void AutoClicker::Click()
	{
		this->data.Score += this->data.ClickValue;
	}

	bool AutoClicker::IsOver()
	{
		return this->data.Score >= this->data.TargetScore;
	}

	std::ostream& operator<<(std::ostream& stream, const AutoClicker* c)
	{
		stream << "Score : " << c->data.Score << " Speed : " << c->data.PassiveSpeed << " Target : " << c->data.TargetScore;
		return stream;
	}
}
